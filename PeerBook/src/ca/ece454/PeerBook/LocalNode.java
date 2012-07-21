/**
 * Group: 14 
 */

package ca.ece454.PeerBook;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.UnknownHostException;
import java.security.NoSuchAlgorithmException;
import java.util.AbstractMap.SimpleEntry;
import java.util.Arrays;
import java.util.Iterator;
import java.util.UUID;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import java.util.logging.Logger;

import ca.ece454.PeerBook.Message.MessageType;
import ca.ece454.PeerBook.Util.CircularBuffer;
import ca.ece454.PeerBook.Util.Util;

public class LocalNode implements Runnable {
	private static final Logger log = Logger.getLogger(LocalNode.class.getName());
	
	private static final String NODES_LIST_FILEPATH = "nodes_list.txt";
	
	private final Server server;
	private final BlockingQueue<SimpleEntry<UUID, Message>> messageQueue;
	private boolean isResourceConstrained;	
	private final Lock lock = new ReentrantLock();
	private final Condition fileAvailable = lock.newCondition();

	private final OnReceiveListener onReceiveCallback = new OnReceiveListener() {
		@Override
		public void onReceive(UUID nodeID, Message message) {
			log.info("onReceive(): Received message, adding to message queue");
			try {
				messageQueue.put(new SimpleEntry<UUID, Message>(nodeID, message));
			} catch (InterruptedException e) {
				log.severe(e.toString());
			}
		}
	};

	/**
	 * Constructor
	 * 
	 * @param port
	 *            The port number on which the local node will listen for
	 *            connections from other nodes.
	 */
	public LocalNode(int port, boolean isResourceConstrained) {
		this.server = new Server(port, onReceiveCallback);
		this.messageQueue = new LinkedBlockingQueue<SimpleEntry<UUID, Message>>();
		this.isResourceConstrained = isResourceConstrained;
	}

	/**
	 * Starts the server, allowing it to begin receiving connections from other
	 * nodes.
	 */
	public void startServer() {
		try {
			server.start();
		} catch (IOException e) {
			log.severe(e.toString());
		}
	}
	
	/**
	 * Stops the server, preventing any new nodes from connecting to the local node.
	 */
	public void stopServer() {
		try {
			server.stop();
		} catch (IOException e) {
			log.severe(e.toString());
		}
	}

	@Override
	public void run() {
		CircularBuffer<UUID> receivedMessageLog = new CircularBuffer<UUID>(100);
		
		while (true) {
			try {
				// Pop an entry off the top of the message queue
				SimpleEntry<UUID, Message> queueEntry = messageQueue.take();
				Message message = queueEntry.getValue();
				
				// Only process messages that have not been received before
				if (!receivedMessageLog.contains(message.getMessageID())) {
					receivedMessageLog.add(message.getMessageID());
					
					// Rebroadcast the message if necessary
					if (message.isRebroadcast())
						NodeManager.getInstance().reBroadcastMessage(queueEntry);
					
					processMessage(queueEntry);
				}
			} catch (InterruptedException e) {
				log.severe(e.toString());
			} catch (IOException e) {
				log.severe(e.toString());
			}
		}
	}

	// ----- Message processing -----
	/**
	 * Process the given message.
	 * 
	 * @param queueEntry
	 *            The key value pair containing the ID of the node that received
	 *            the message and the received message.
	 */
	private void processMessage(SimpleEntry<UUID, Message> queueEntry) {
		log.info("Received: " + queueEntry.getValue().getMessageType());
		
		switch (queueEntry.getValue().getMessageType()) {
		case DOWNLOAD_REQUEST:
			processDownloadRequest(queueEntry);			
			break;
		case FILE_CHANGE_NOTIFICATION:
			processFileChangeNotification(queueEntry);			
			break;
		case SYNCHRONIZATION_RESPONSE:
			processSyncrhonizationResponse(queueEntry);
			break;
		case NEW_FILE_NOTIFICATION:
			processNewFileNotification(queueEntry);
			break;
		case REMOVE_FILE_NOTIFICATION:
			processRemoveFileNotification(queueEntry);
			break;
		case SYNCHRONIZATION_REQUEST:
			processSynchronizationRequest(queueEntry);
			break;
		case TAG_NOTIFICATION:
			processTagNotification(queueEntry);
			break;
		case DOWNLOAD_RESPONSE:
			processDownloadResponse(queueEntry);
			break;
		default:
			break;
		}
	}

	private void processDownloadRequest(SimpleEntry<UUID, Message> queueEntry) {
		FileMetadata metadata = queueEntry.getValue().getFileMetadata(0);
		PeerBookFile file = FileManager.getInstance().getFile(metadata.getFilename());
		
		if (file == null || !file.getFileMetadata().isValid())
			return;
		else if (file.getFileMetadata().isAvailableLocally()) {
			// Construct response message with the data of the file requested for download
			Message responseMessage = new Message(MessageType.DOWNLOAD_RESPONSE, false);
			responseMessage.setMessageID(queueEntry.getValue().getMessageID());
			responseMessage.addFileMetadata(file.getFileMetadata());
			responseMessage.setFileData(file.read());
			
			try {
				NodeManager.getInstance().getNode(queueEntry.getKey()).sendMessage(responseMessage);
			} catch (IOException e) {
				log.severe(e.toString());
			}
		}
	}
	
	private void processFileChangeNotification(SimpleEntry<UUID, Message> queueEntry) {
		FileMetadata newMetadata = queueEntry.getValue().getFileMetadata(0);
		PeerBookFile file = FileManager.getInstance().getFile(newMetadata.getFilename());
		
		if (file == null) {
			// File change notification for a newly created file
			file = new PeerBookFile(newMetadata);
			FileManager.getInstance().addFile(file);
		} else if (file.getFileMetadata().getInternalVersion() <= newMetadata.getInternalVersion()) {
			// Update necessary local metadata information
			file.getFileMetadata().setInternalVersion(newMetadata.getInternalVersion());
			file.getFileMetadata().setChecksum(newMetadata.getChecksum());
			file.getFileMetadata().setLastModified(newMetadata.getLastModified());
			
			// Mark local copy, if it exists, as invalid
			file.getFileMetadata().setValid(false);
		} else
			return;
		
		// Send download request
		if (file.getFileMetadata().isKeepLocalCopy()) {
			Message responseMessage = new Message(MessageType.DOWNLOAD_REQUEST, false);
			responseMessage.addFileMetadata(newMetadata);
			
			try {
				NodeManager.getInstance().getNode(queueEntry.getKey()).sendMessage(responseMessage);
			} catch (IOException e) {
				log.severe(e.toString());
			}
		}
	}
	
	private void processSyncrhonizationResponse(SimpleEntry<UUID, Message> queueEntry) {
		FileManager fileManager = FileManager.getInstance();
		RemoteNode node = NodeManager.getInstance().getNode(queueEntry.getKey());
		Iterator<FileMetadata> iterator = queueEntry.getValue().getFilesListIterator();
		
		fileManager.invalidateAllFiles();
		
		while (iterator.hasNext()) {
			FileMetadata remoteFileMetadata = iterator.next();
			PeerBookFile file = fileManager.getFile(remoteFileMetadata.getFilename());
			
			if (file != null) {
				FileMetadata localFileMetadata = file.getFileMetadata();
				if (remoteFileMetadata.getInternalVersion() >= localFileMetadata.getInternalVersion()) {
					// Remote file is newer or versions are equal so network file given precedence
					// Update file metadata with data from message, but mark it
					// as unavailable locally
					file.setFileMetadata((FileMetadata) remoteFileMetadata.clone());
					file.getFileMetadata().setAvailableLocally(false);
					file.getFileMetadata().setValid(true);
					
					Message downloadRequestMessage = new Message(MessageType.DOWNLOAD_REQUEST, false);
					downloadRequestMessage.addFileMetadata(remoteFileMetadata);
					
					// Send download request to node
					try {
						node.sendMessage(downloadRequestMessage);
					} catch (IOException e) {
						log.severe(e.toString());
					}
				} else {
					// Local file is newer
					localFileMetadata.setValid(true);
					
					MessageType messageType = localFileMetadata.isDeleted() ? MessageType.REMOVE_FILE_NOTIFICATION
							: MessageType.FILE_CHANGE_NOTIFICATION;
					
					Message fileChangeMessage = new Message(messageType, true);
					fileChangeMessage.addFileMetadata(localFileMetadata);
					
					// Broadcast to all other nodes
					try {
						NodeManager.getInstance().broadcastMessage(fileChangeMessage);
					} catch (IOException e) {
						log.severe(e.toString());
					}
				}
			} else {
				// New file which needs to be added to local file manager and
				// downloaded from remote node
				FileMetadata localFileMetadata = new FileMetadata(
						remoteFileMetadata.getFilename(), 
						remoteFileMetadata.getDirectory(), 
						false, 
						true, 
						true, 
						false, 
						remoteFileMetadata.getLastModified(), 
						remoteFileMetadata.getInternalVersion());
				
				fileManager.addFile(new PeerBookFile(localFileMetadata));
				Message downloadRequestMessage = new Message(MessageType.DOWNLOAD_REQUEST, false);
				downloadRequestMessage.addFileMetadata(localFileMetadata);
				
				// Send download request to node
				try {
					node.sendMessage(downloadRequestMessage);
				} catch (IOException e) {
					log.severe(e.toString());
				}
			}
		}
		
		// Delete all remaining invalid files
		fileManager.deleteInvalidFiles();
	}
	
	private void processNewFileNotification(SimpleEntry<UUID, Message> queueEntry) {
		FileMetadata newMetadata = queueEntry.getValue().getFileMetadata(0);
		PeerBookFile file = new PeerBookFile(newMetadata);
		FileManager.getInstance().addFile(file);
		
		if (file.getFileMetadata().isKeepLocalCopy()) {
			// Send download request
			Message responseMessage = new Message(MessageType.DOWNLOAD_REQUEST, false);
			responseMessage.addFileMetadata(newMetadata);
			
			try {
				NodeManager.getInstance().getNode(queueEntry.getKey()).sendMessage(responseMessage);
			} catch (IOException e) {
				log.severe(e.toString());
			}
		}
	}
	
	private void processRemoveFileNotification(SimpleEntry<UUID, Message> queueEntry) {
		FileMetadata remoteFileMetadata = queueEntry.getValue().getFileMetadata(0);
		PeerBookFile file = FileManager.getInstance().getFile(remoteFileMetadata.getFilename());
		FileMetadata localFileMetadata = file.getFileMetadata();
		
		if (remoteFileMetadata.getInternalVersion() > localFileMetadata.getInternalVersion()) {
			FileManager.getInstance().deleteFile(localFileMetadata.getFilename());
			FileManager.getInstance().removeFile(file);
		}
	}
	
	private void processSynchronizationRequest(SimpleEntry<UUID, Message> queueEntry) {
		Message responseMessage = new Message(MessageType.SYNCHRONIZATION_RESPONSE, false);
		Iterator<PeerBookFile> iterator = FileManager.getInstance().getFilesIterator();
		
		while (iterator.hasNext()) {
			responseMessage.addFileMetadata(iterator.next().getFileMetadata());
		}
		
		try {
			NodeManager.getInstance().getNode(queueEntry.getKey()).sendMessage(responseMessage);
		} catch (IOException e) {
			log.severe(e.toString());
		}
	}
	
	private void processTagNotification(SimpleEntry<UUID, Message> queueEntry) {
		FileMetadata remoteFileMetadata = queueEntry.getValue().getFileMetadata(0);
		PeerBookFile file = FileManager.getInstance().getFile(remoteFileMetadata.getFilename());
		
		if (file != null) {
			try {
				file.tag();
			} catch (IOException e) {
				log.severe(e.toString());
			}
		}
	}
	
	private void processDownloadResponse(SimpleEntry<UUID, Message> queueEntry) {
		FileMetadata remoteFileMetadata = queueEntry.getValue().getFileMetadata(0);
		PeerBookFile file = FileManager.getInstance().getFile(remoteFileMetadata.getFilename());
		
		file.write(queueEntry.getValue().getFileData());
		file.setFileMetadata(remoteFileMetadata);
		
		// Generate new checksum based on new file contents
		try {
			file.generateChecksum(Util.CHECKSUM_ALGORITHM);
		} catch (NoSuchAlgorithmException e) {
			log.severe(e.toString());
		}
		
		notify();
	}
	// ----- End of message processing -----
	
	/**
	 * Send synchronization request to a node in the network
	 */
	private void sendSynchronizationRequest() {
		if (!NodeManager.getInstance().isConnected()) {
			log.warning("Not connected to network, failed to send synchronization request");
			return;
		}
		
		Message message = new Message(MessageType.SYNCHRONIZATION_REQUEST, false);
		RemoteNode node = NodeManager.getInstance().getNode();
		
		try {
			node.sendMessage(message);
		} catch (IOException e) {
			log.warning(e.toString());
		}
	}

	// ----- Public interface -----
	public void join() {
		try {
			NodeManager.getInstance().connectToNodes(NODES_LIST_FILEPATH);
		} catch (UnknownHostException e) {
			log.severe(e.toString());
		} catch (IOException e) {
			log.severe(e.toString());
		}
		
		sendSynchronizationRequest();
	}

	public void leave() {
		NodeManager.getInstance().disconnectFromNodes();
	}

	public void addFile(String filepath) {		
		FileMetadata fileMetadata = new FileMetadata(
				Util.extractFilename(filepath),
				Util.extractDirectory(filepath), true, true, true, false, 0, 0);
		PeerBookFile file = new PeerBookFile(fileMetadata);
		FileManager.getInstance().addFile(file);
		
		try {
			file.updateChecksum(Util.CHECKSUM_ALGORITHM);
			
			// Send notification to other nodes
			Message message = new Message(MessageType.NEW_FILE_NOTIFICATION, true);
			message.addFileMetadata(fileMetadata);
		
			NodeManager.getInstance().broadcastMessage(message);
		} catch (NoSuchAlgorithmException e) {
			log.warning(e.toString());
		} catch (IOException e) {
			log.severe(e.toString());
		}
	}

	public BufferedOutputStream createFile(String filepath) {		
		FileMetadata fileMetadata = new FileMetadata(
				Util.extractFilename(filepath),
				Util.extractDirectory(filepath), true, true, true, false, 0, 0);
		PeerBookFile file = new PeerBookFile(fileMetadata);
		FileManager.getInstance().addFile(file);
		
		return writeFile(filepath);
	}
	
	public BufferedInputStream readFile(String filepath) {
		String filename = Util.extractFilename(filepath);
		PeerBookFile file = FileManager.getInstance().getFile(filename);
		
		if (file == null)
			return null;
		
		// Download file if not available locally
		if (!file.getFileMetadata().isAvailableLocally()) {
			Message message = new Message(MessageType.DOWNLOAD_REQUEST, true);
			message.addFileMetadata(file.getFileMetadata());
			
			lock.lock();
			try {
				NodeManager.getInstance().broadcastMessage(message);
				
				// Wait for file download
				fileAvailable.await(250, TimeUnit.MILLISECONDS);
			} catch (IOException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} finally {
				lock.unlock();
			}
			
			if (!file.getFileMetadata().isAvailableLocally())
				return null;
		}
		
		BufferedInputStream input = null;		
		try {
			input = file.getBufferedInputStream();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		return input;
	}
	
	public BufferedOutputStream writeFile(String filepath) {
		// TODO REturn stream to allow for writing to file
		String filename = Util.extractFilename(filepath);
		PeerBookFile file = FileManager.getInstance().getFile(filename);
		
		if (file == null)
			return null;
		
		BufferedOutputStream output = null;
		try {
			output = file.getBufferedOutputStream();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		
		return output;
	}
	
	public void closeFile(String filepath) {
		String filename = Util.extractFilename(filepath);
		PeerBookFile file = FileManager.getInstance().getFile(filename);
		
		try {
			byte[] newChecksum = file.generateChecksum(Util.CHECKSUM_ALGORITHM);
			if (!Arrays.equals(newChecksum, file.getFileMetadata().getChecksum())) {
				file.getFileMetadata().setChecksum(newChecksum);
				file.getFileMetadata().setInternalVersion(
						file.getFileMetadata().getInternalVersion() + 1);
				file.getFileMetadata().setLastModified(System.currentTimeMillis());
				file.getFileMetadata().setValid(true);
				
				Message message = new Message(MessageType.FILE_CHANGE_NOTIFICATION, true);
				message.addFileMetadata(file.getFileMetadata());
				NodeManager.getInstance().broadcastMessage(message);
			}
		} catch (NoSuchAlgorithmException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void deleteFile(String filepath) {
		String filename = Util.extractFilename(filepath);
		PeerBookFile file = FileManager.getInstance().getFile(filename);
		
		if (file == null)
			return;
		
		FileManager.getInstance().deleteFile(filename);
		
		Message message = new Message(MessageType.REMOVE_FILE_NOTIFICATION, true);
		message.addFileMetadata(file.getFileMetadata());
		
		if (NodeManager.getInstance().isConnected()) {
			try {
				NodeManager.getInstance().broadcastMessage(message);
			} catch (IOException e) {
				log.severe(e.toString());
			}
			
			// Remove from file manger if connected to network, otherwise keep
			// for synchronization purposes
			FileManager.getInstance().removeFile(file);
		}
	}

	public void listFiles() {
		Iterator<PeerBookFile> iterator = FileManager.getInstance().getFilesIterator();
		
		System.out.println("----- Files currently in PeerBook: -----");
		System.out.printf("%-30s %-50s %30.30s %20s %10s %n", "FILENAME", "FILEPATH", "VERSION", "AVAILABLE LOCALLY", "READONLY");
		
		while (iterator.hasNext()) {
			PeerBookFile file = iterator.next();
			FileMetadata fileMetadata = file.getFileMetadata();
			
			if (fileMetadata.isVersionedFile() || fileMetadata.isDeleted())
				continue;
			
			System.out.printf("%-30s %-50s %30.30s %20s %10s %n",
					fileMetadata.getFilename(), fileMetadata.getFilepath(),
					fileMetadata.getCurrentUserTag(),
					fileMetadata.isAvailableLocally(),
					fileMetadata.isReadOnly());
		}
		System.out.println();
	}

	public void listFileVersions(String filepath) {
		String filename = Util.extractFilename(filepath);
		System.out.println("----- Tagged versions for " + filepath + ": -----");
		System.out.printf("%-30s %-50s %30.30s %20s %10s %n", "FILENAME", "FILEPATH", "VERSION", "AVAILABLE LOCALLY", "READONLY");
		
		PeerBookFile file = FileManager.getInstance().getFile(filename);
		
		if (file == null)
			return;
		
		FileMetadata origFileMetadata = file.getFileMetadata();
		
		for (UserTag userTag : origFileMetadata.getUserTaggedVersions()) {
			PeerBookFile versionedFile = FileManager.getInstance().getFile(userTag.getFilename());
			FileMetadata versionedFileMetadata = versionedFile.getFileMetadata();
			
			System.out.printf("%-30s %-50s %30.30s %20s %10s %n",
					versionedFileMetadata.getFilename(),
					versionedFileMetadata.getFilepath(), userTag.getVersion(),
					versionedFileMetadata.isAvailableLocally(),
					versionedFileMetadata.isReadOnly());
		}
		System.out.println();
	}
	
	public void tagFile(String filepath) {
		String filename = Util.extractFilename(filepath);
		
		PeerBookFile file = FileManager.getInstance().getFile(filename);
		
		if (file == null)
			return;
		
		try {
			file.tag();
			
			Message message = new Message(MessageType.TAG_NOTIFICATION, true);
			message.addFileMetadata(file.getFileMetadata());
			NodeManager.getInstance().broadcastMessage(message);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	// ----- End of public interface -----

	// Getters / setters
	public boolean isResourceConstrained() {
		return isResourceConstrained;
	}

	public void setResourceConstrained(boolean isResourceConstrained) {
		this.isResourceConstrained = isResourceConstrained;
	}
}
