package ca.ece454.PeerBook;

import java.util.HashMap;

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
	 * Adds the given file to the files table.
	 * 
	 * @param file
	 *            The file to be added.
	 */
	public void addFile(PeerBookFile file) {
		filesTable.put(file.getFileMetadata().getFilepath(), file);
	}

	/**
	 * Removes the file from the table of managed files, if it exists.
	 * 
	 * @param file
	 *            The file to be removed.
	 */
	public void removeFile(PeerBookFile file) {
		filesTable.remove(file.getFileMetadata().getFilepath());
	}
	
	/**
	 * Returns a reference to a file object associated with the given file path.
	 * @param filepath The file path used to look up the file object.
	 * @return The file object if it is found, null otherwise.
	 */
	public PeerBookFile getFile(String filepath) {
		return filesTable.get(filepath);
	}
}
