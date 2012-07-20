package ca.ece454.PeerBook.Util;

import java.io.File;

public class Util {
	public static final String FILE_SEPERATOR = File.separator;
	public static final String VERSION_DIRECTORY = "versioned_files";
	public static final String CHECKSUM_ALGORITHM = "MD5";
	
	/**
	 * Returns the hex representation of the given checksum.
	 * @param checksum The checksum to be printed.
	 * @return A hex representation of the given checksum as a string.
	 */
	public static String checksumString(byte[] checksum) {
		String result = "";
		
		for (int i = 0; i < checksum.length; i++) {
			result += Integer.toString((checksum[i] & 0xff)  + 0x100, 16).substring(1);
		}
		
		return result;
	}
	
	/**
	 * Extracts the filename from the given file path.
	 * @param filepath A valid file path.
	 * @return The file name.
	 */
	public static String extractFilename(String filepath) {
		int position = filepath.lastIndexOf(FILE_SEPERATOR);
		
		position = position < 0 ? 0 : position++;
		return filepath.substring(position);
	}
	
	/**
	 * Extracts the directory from the given file path. Includes everything except the
	 * file name.
	 * @param filepath A valid file path.
	 * @return The directory of the file path.
	 */
	public static String extractDirectory(String filepath) {
		int position = filepath.lastIndexOf(FILE_SEPERATOR);
		
		return (position < 0) ? "" : filepath.substring(0, position);		
	}
}
