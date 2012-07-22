package ca.ece454.PeerBook;

import java.util.HashMap;
import java.util.Iterator;

public class FileManager {	
	private static FileManager instance;

	private HashMap<String, PeerBookFile> filesTable;

	private FileManager() {
		this.filesTable = new HashMap<String, PeerBookFile>();
	}

	public static synchronized FileManager getInstance() {
		if (instance == null)
			instance = new FileManager();
		return instance;
	}
	
	/**
	 * Checks whether the given filename is managed by the FileManager.
	 * 
	 * @param filename
	 *            The name of file to check.
	 * @return True if the FileManager contains the file.
	 */
	public boolean containsFile(String filename) {
		return filesTable.containsKey(filename);
	}

	/**
	 * Adds the given file to the files table.
	 * 
	 * @param file
	 *            The file to be added.
	 */
	public void addFile(PeerBookFile file) {		
		filesTable.put(file.getFileMetadata().getFilename(), file);
	}

	/**
	 * Removes the file from the table of managed files, if it exists.
	 * 
	 * @param file
	 *            The file to be removed.
	 */
//	public void removeFile(PeerBookFile file) {
//		filesTable.remove(file.getFileMetadata().getFilename());
//	}
	
	/**
	 * Deletes the given file if it exists. The file is not removed from the
	 * manager for synchronization purposes. To remove the file from the manager
	 * call removeFile().
	 * 
	 * @param filename The name of the file to delete.
	 */
	public void deleteFile(String filename) {
		PeerBookFile file = filesTable.get(filename);
		
		if (file != null)
			file.delete();
	}
	
	/**
	 * Returns a reference to a file object associated with the given file path.
	 * 
	 * @param filename
	 *            The filename used to look up the file object.
	 * @return The file object if it is found, null otherwise.
	 */
	public PeerBookFile getFile(String filename) {
		return filesTable.get(filename);
	}
	
	public Iterator<PeerBookFile> getFilesIterator() {
		return filesTable.values().iterator();
	}
	
	/**
	 * Marks all local files as invalid to prepare for synchronization.
	 */
	public void invalidateAllFiles() {
		for (PeerBookFile file : filesTable.values()) {
			file.getFileMetadata().setValid(false);
		}
	}
	
	/**
	 * Deletes all files marked as invalid. Files are deleted physically and
	 * also removed from the manager.
	 */
	public void deleteInvalidFiles() {
		Iterator<PeerBookFile> iterator = filesTable.values().iterator();
		
		while (iterator.hasNext()) {
			PeerBookFile file = iterator.next();
			if (!file.getFileMetadata().isValid()) {
				file.delete();
				iterator.remove();
			}	
		}
	}
	
	/**
	 * Removes any file marked as deleted from the file manager.
	 */
//	public void removeDeletedFiles() {
//		Iterator<PeerBookFile> iterator = filesTable.values().iterator();
//
//		while (iterator.hasNext()) {
//			PeerBookFile file = iterator.next();
//			if (!file.getFileMetadata().isDeleted()) {
//				iterator.remove();
//			}
//		}
//	}
}
