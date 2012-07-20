/**
 * Group: 14 
 */

package ca.ece454.PeerBook;

import java.io.IOException;
import java.net.UnknownHostException;
import java.security.NoSuchAlgorithmException;
import java.util.AbstractMap.SimpleEntry;
import java.util.UUID;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.logging.Logger;

import ca.ece454.PeerBook.Message.MessageType;
import ca.ece454.PeerBook.Util.CircularBuffer;

public class LocalNode implements Runnable {
	private static final Logger log = Logger.getLogger(LocalNode.class.getName());
	
	private static final String NODES_LIST_FILEPATH = "nodes_list.txt";
	
	private final Server server;
	private final BlockingQueue<SimpleEntry<UUID, Message>> messageQueue;

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
	public LocalNode(int port) {
		this.server = new Server(port, onReceiveCallback);
		this.messageQueue = new LinkedBlockingQueue<SimpleEntry<UUID, Message>>();
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
		PeerBookFile file = FileManager.getInstance().getFile(metadata.getFilepath());
		
		if (file == null)
			return;
		else if (file.getFileMetadata().isAvailableLocally()) {
			// Construct response message with the data of the file requested for download
			Message responseMessage = new Message(MessageType.DOWNLOAD_RESPONSE, false);
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
		PeerBookFile file = FileManager.getInstance().getFile(newMetadata.getFilepath());
		
		if (file == null || file.getFileMetadata().getInternalVersion() >= newMetadata.getInternalVersion())
			return;
		
		// Update necessary local metadata information
		file.getFileMetadata().setInternalVersion(newMetadata.getInternalVersion());
		file.getFileMetadata().setChecksum(newMetadata.getChecksum());
		file.getFileMetadata().setLastModified(newMetadata.getLastModified());
		
		// Invalidate local copy, if its available
		if (file.getFileMetadata().isAvailableLocally())
			file.getFileMetadata().setValid(false);
		
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
		// TODO
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
		// TODO
	}
	
	private void processSynchronizationRequest(SimpleEntry<UUID, Message> queueEntry) {
		// TODO
	}
	
	private void processTagNotification(SimpleEntry<UUID, Message> queueEntry) {
		// TODO
	}
	
	private void processDownloadResponse(SimpleEntry<UUID, Message> queueEntry) {
		FileMetadata messageFileMetadata = queueEntry.getValue().getFileMetadata(0);
		PeerBookFile file = FileManager.getInstance().getFile(messageFileMetadata.getFilepath());
		
		file.write(queueEntry.getValue().getFileData());
		
		// Generate new checksum based on new file contents
		try {
			file.generateChecksum("MD5");
		} catch (NoSuchAlgorithmException e) {
			log.severe(e.toString());
		}
	}
	// ----- End of message processing -----

	// ----- Public interface -----
	public void join() {
		try {
			NodeManager.getInstance().connectToNodes(NODES_LIST_FILEPATH);
		} catch (UnknownHostException e) {
			log.severe(e.toString());
		} catch (IOException e) {
			log.severe(e.toString());
		}
	}

	public void leave() {
		// TODO Leave network. Push pending updates?
		NodeManager.getInstance().disconnectFromNodes();
	}

	public void addFile(String filepath) {
		// TODO Add file
	}

	public void createFile(String filepath) {
		// TODO Create file
	}

	public void deleteFile(String filepath) {
		// TODO Delete file
	}

	public void listFiles() {
		// TODO List files
	}

	public void listFileVersions(String filepath) {
		// TODO List all versions of the given file
	}

	public void openFile(String filepath) {
		// TODO Open file and provide some way to read/write to the file
		// might need to implement a stream wrapper
	}

	public void closeFile(String filepath) {
		// TODO Close the file. Performing the necessary synchronization
		// operations.
	}
	// ----- End of public interface -----
}
