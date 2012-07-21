package ca.ece454.PeerBook;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardCopyOption;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.logging.Logger;

import ca.ece454.PeerBook.Util.Util;

public class PeerBookFile {
	private static final Logger log = Logger.getLogger(PeerBookFile.class.getName());

	private FileMetadata fileMetadata;
	
	public PeerBookFile(String filename, String directory, boolean isAvailableLocally, boolean keepLocalCopy, boolean isValid, boolean isReadOnly) {
		this.fileMetadata = new FileMetadata(filename, directory, isAvailableLocally, keepLocalCopy, isValid, isReadOnly, 0, 0);
	}

	public PeerBookFile(FileMetadata fileMetadata) {
		this.fileMetadata = fileMetadata;
	}
	
	public InputStream getInputStream() {
		// TODO Modify to work with custom input stream
		return null;
	}
	
	public OutputStream getOutputStream() {
		// TODO Modify to work with custom output stream
		return null;
	}
	
	/**
	 * Returns the binary data of the file as an array if the file is available
	 * locally.
	 * 
	 * @return The binary data of the file as a byte array if it is available
	 *         locally.
	 */
	public byte[] read() {
		if (!fileMetadata.isAvailableLocally())
			return null;
		
		File file = new File(fileMetadata.getDirectory() + Util.FILE_SEPERATOR + fileMetadata.getFilename());
		byte[] result = new byte[(int)file.length()];
				
		try {
			InputStream input = null;
			
			try {
				input = new BufferedInputStream(new FileInputStream(file));
				int totalBytesRead = 0;
				while (totalBytesRead < result.length) {
					int bytesRead = input.read(result, totalBytesRead, result.length - totalBytesRead);
					
					if (bytesRead > 0)
						totalBytesRead += bytesRead;
					else
						break;
				}
			} finally {
				if (input != null)
					input.close();
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (IOException e) {
			log.severe(e.toString());
		}
		
		return result;
	}
	
	/**
	 * Writes the given byte array to the file.
	 * 
	 * @param data
	 *            The data to be written to file in the form of a byte array.
	 */
	public void write(byte[] data) {
		if (fileMetadata.isReadOnly())
			return;
		
		try {
			OutputStream output = null;
			
			try {
				output = new BufferedOutputStream(new FileOutputStream(fileMetadata.getFilepath(), false));
				output.write(data);
			} finally {
				output.close();
			}			
		} catch (FileNotFoundException e) {
			log.severe(e.toString());
		} catch (IOException e) {
			log.severe(e.toString());
		}
	}
	
	/**
	 * Delete the physical file from the system.
	 * 
	 * @return True if the file was deleted.
	 */
	public boolean delete() {
		// Update file metadata
		fileMetadata.setAvailableLocally(false);
		fileMetadata.setInternalVersion(fileMetadata.getInternalVersion() + 1);
		fileMetadata.setDeleted(true);
		
		File file = new File(fileMetadata.getFilepath());
		return file.delete();
	}
	
	/**
	 * Generate a checksum of the file based on the file contents.
	 * 
	 * @param algorithm
	 *            The hashing algorithm to use. (MD5, SHA-1, SHA-256, ...)
	 * @throws NoSuchAlgorithmException
	 */
	public byte[] generateChecksum(String algorithm) throws NoSuchAlgorithmException {
		byte[] fileData = read();
		MessageDigest md = MessageDigest.getInstance(algorithm);
		return md.digest(fileData);
	}
	
	/**
	 * Updates the file's checksum.
	 * 
	 * @param algorithm
	 *            The hashing algorithm to use for checksum generation.
	 * @throws NoSuchAlgorithmException
	 */
	public void updateChecksum(String algorithm) throws NoSuchAlgorithmException {
		fileMetadata.setChecksum(generateChecksum(algorithm));
	}
	
	/**
	 * Creates a new user tagged version of the file. This involves creating a
	 * hidden read-only copy of the file and updating and storing the user tag
	 * data.
	 * 
	 * @throws IOException
	 */
	public void tag() throws IOException {
		int newTagVersion = fileMetadata.getCurrentUserTag() + 1;
		String tagFilepath = Util.generateTagFilepath(fileMetadata.getFilepath(), newTagVersion);
		
		// Create a copy of the file being tagged
		Path source = FileSystems.getDefault().getPath(fileMetadata.getDirectory(), fileMetadata.getFilename());
		Path destination = FileSystems.getDefault().getPath(tagFilepath, "");
		Files.copy(source, destination, StandardCopyOption.REPLACE_EXISTING);
		
		// Populate the relevant user tag information
		UserTag tag = new UserTag(
				"." + fileMetadata.getFilename() + "." + newTagVersion,
				tagFilepath,
				newTagVersion, 
				System.currentTimeMillis());
		
		fileMetadata.addUserTag(tag);
		
		// Populate metadata for the newly tagged file
		FileMetadata tagMetadata = new FileMetadata(
				tag.getFilename(), 
				Util.extractDirectory(tagFilepath),
				true, true, true, true, 0, 0);
		tagMetadata.setVersionedFile(true);
		PeerBookFile file = new PeerBookFile(tagMetadata);
		
		try {
			file.updateChecksum(Util.CHECKSUM_ALGORITHM);
		} catch (NoSuchAlgorithmException e) {
			log.warning(e.toString());
		}
		
		// Add versioned file to the table of managed files
		FileManager.getInstance().addFile(file);
		
		// Increment internal version number
		fileMetadata.setInternalVersion(fileMetadata.getInternalVersion() + 1);
	}
	
	public BufferedInputStream getBufferedInputStream() throws FileNotFoundException {
		File file = new File(fileMetadata.getDirectory() + Util.FILE_SEPERATOR + fileMetadata.getFilename());
		return new BufferedInputStream(new FileInputStream(file));
	}
	
	public BufferedOutputStream getBufferedOutputStream() throws FileNotFoundException {
		File file = new File(fileMetadata.getDirectory() + Util.FILE_SEPERATOR + fileMetadata.getFilename());
		return new BufferedOutputStream(new FileOutputStream(file));
	}

	// Getters / setters
	public FileMetadata getFileMetadata() {
		return fileMetadata;
	}
	
	public void setFileMetadata(FileMetadata fileMetadata) {
		this.fileMetadata = fileMetadata;
	}
}
