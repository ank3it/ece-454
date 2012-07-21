package ca.ece454.PeerBook;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.UUID;

public class Message implements Serializable {
	private static final long serialVersionUID = 282768819703406686L;
	
	public enum MessageType {
		FILE_CHANGE_NOTIFICATION,
		NEW_FILE_NOTIFICATION,
		REMOVE_FILE_NOTIFICATION,
		DOWNLOAD_REQUEST,
		TAG_NOTIFICATION,
		SYNCHRONIZATION_REQUEST,
		SYNCHRONIZATION_RESPONSE,
		DOWNLOAD_RESPONSE
	}
	
	private UUID messageID;
	private MessageType messageType;
	private boolean rebroadcast;
	private ArrayList<FileMetadata> filesList;
	private byte[] fileData;

	/**
	 * Constructor
	 * @param messageType The type of the message
	 * @param rebroadcast True if the message should be rebroadcast when received.
	 */
	public Message(MessageType messageType, boolean rebroadcast) {
		this.messageID = UUID.randomUUID();
		this.filesList = new ArrayList<FileMetadata>();
		this.messageType = messageType;
		this.rebroadcast = rebroadcast;
	}
	
	/**
	 * Adds the given file metadata to the message.
	 * @param fileMetadata The file metadata to be added.
	 * @return True if something was added.
	 */
	public boolean addFileMetadata(FileMetadata fileMetadata) {
		if (filesList.contains(fileMetadata))
			return false;
		
		return filesList.add(fileMetadata);
	}
	
	/**
	 * Returns the file metadata stored at the given index.
	 * @param i The index.
	 * @return A FileMetadata object or null if index was out of bounds.
	 */
	public FileMetadata getFileMetadata(int i) {
		if (i >= filesList.size())
			return null;
		
		return filesList.get(i);
	}
	
	/**
	 * Returns an iterator over the files list.
	 * @return An Iterator over the files list.
	 */
	public Iterator<FileMetadata> getFilesListIterator() {
		return filesList.iterator();
	}

	// Getters/setters
	public MessageType getMessageType() {
		return messageType;
	}

	public void setMessageType(MessageType messageType) {
		this.messageType = messageType;
	}

	public UUID getMessageID() {
		return messageID;
	}
	
	public void setMessageID(UUID messageID) {
		this.messageID = messageID;
	}

	public byte[] getFileData() {
		return fileData;
	}

	public void setFileData(byte[] fileData) {
		this.fileData = fileData;
	}

	public boolean isRebroadcast() {
		return rebroadcast;
	}

	public void setRebroadcast(boolean rebroadcast) {
		this.rebroadcast = rebroadcast;
	}
}