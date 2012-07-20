package ca.ece454.PeerBook;

import java.io.Serializable;
import java.util.ArrayList;

import ca.ece454.PeerBook.Util.Util;

public class FileMetadata implements Serializable {
	private static final long serialVersionUID = -205428958059968668L;

	private String filename;
	private String directory;
	private boolean isAvailableLocally;
	private boolean keepLocalCopy;
	private boolean isValid;
	private boolean isReadOnly;
	private long lastModified;
	private int internalVersion;
	private byte[] checksum;
	private ArrayList<UserTag> userTaggedVersions;

	public FileMetadata(String filename, String directory,
			boolean isAvailableLocally, boolean keepLocalCopy, boolean isValid,
			boolean isReadOnly, long lastModified, int internalVersion) {
		this.filename = filename;
		this.directory = directory;
		this.isAvailableLocally = isAvailableLocally;
		this.keepLocalCopy = keepLocalCopy;
		this.isValid = isValid;
		this.isReadOnly = isReadOnly;
		this.lastModified = lastModified;
		this.internalVersion = internalVersion;
		this.userTaggedVersions = new ArrayList<UserTag>();
	}

//	public FileMetadata(String filename, String directory,
//			boolean isAvailableLocally, boolean keepLocalCopy, boolean isValid,
//			boolean isReadOnly, long lastModified, int internalVersion,
//			byte[] checksum) {
//		this(filename, directory, isAvailableLocally, keepLocalCopy, isValid,
//				isReadOnly, lastModified, internalVersion);
//		this.checksum = checksum;
//	}
	
	/**
	 *  Adds the given user tag to the list of tags.
	 * @param userTag The user tag to be added.
	 */
	public void addUserTag(UserTag userTag) {
		userTaggedVersions.add(userTag);
	}

	// Getters and setters
	public String getFilename() {
		return filename;
	}

	public void setFilename(String filename) {
		this.filename = filename;
	}

	public String getDirectory() {
		return directory;
	}

	public void setDirectory(String directory) {
		this.directory = directory;
	}

	public boolean isAvailableLocally() {
		return isAvailableLocally;
	}

	public void setAvailableLocally(boolean isAvailableLocally) {
		this.isAvailableLocally = isAvailableLocally;
	}

	public boolean isKeepLocalCopy() {
		return keepLocalCopy;
	}

	public void setKeepLocalCopy(boolean keepLocalCopy) {
		this.keepLocalCopy = keepLocalCopy;
	}

	public boolean isValid() {
		return isValid;
	}

	public boolean isReadOnly() {
		return isReadOnly;
	}

	public void setValid(boolean isValid) {
		this.isValid = isValid;
	}

	public long getLastModified() {
		return lastModified;
	}

	public void setLastModified(long lastModified) {
		this.lastModified = lastModified;
	}

	public int getInternalVersion() {
		return internalVersion;
	}

	public void setInternalVersion(int internalVersion) {
		this.internalVersion = internalVersion;
	}
	
	public int getCurrentUserTag() {
		return userTaggedVersions.size();
	}

	public byte[] getChecksum() {
		return checksum;
	}

	public void setChecksum(byte[] checksum) {
		this.checksum = checksum;
	}

	public String getFilepath() {
		return directory + Util.FILE_SEPERATOR + filename;
	}
}
