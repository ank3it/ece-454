package ca.ece454.PeerBook.Test;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;

import ca.ece454.PeerBook.LocalNode;


public class SystemAPITest {
	private static final String FILE_TEST_COPY = "files\\test - Copy.jpg";
	private static final String FILE_TEST = "files\\test.jpg";
	private static final String FILE_ABC = "files\\abc.txt";

	public static void main(String[] args) throws IOException {
		LocalNode ln = new LocalNode(8080, false);
		
		// Add exiting files
		ln.addFile(FILE_TEST);
		ln.addFile(FILE_TEST_COPY);
		
		// Create new file
		BufferedOutputStream out = ln.createFile(FILE_ABC);
		String str = "This file was created by SystemAPITest!";
		out.write(str.getBytes());
		out.close();
		ln.closeFile(FILE_ABC);
		
		// List files
		ln.listFiles();
		
		// Delete "test - Copy.jpg"
		System.out.println("Deleting " + FILE_TEST_COPY);
		ln.deleteFile(FILE_TEST_COPY);
		
		// List files
		ln.listFiles();		
		
		// Tag file abc.txt
		System.out.println("Tagging " + FILE_ABC + " as stable version");
		ln.tagFile(FILE_ABC);
		
		// List versions for abc.txt
		ln.listFileVersions(FILE_ABC);
		
		// Write to abc.txt
		System.out.println("Writing to abc.txt");
		out = ln.writeFile(FILE_ABC);
		out.write(("Hello!").getBytes());
		out.close();
		ln.closeFile(FILE_ABC);
		
		// Tag abc.txt again
		System.out.println("Tagging abc.txt as stable version again");
		ln.tagFile(FILE_ABC);
		
		// List files
		ln.listFiles();
		
		// Reading from abc.txt
		BufferedInputStream in = ln.readFile(FILE_ABC);
		byte[] b = new byte[100];
		in.read(b, 0, 100);
		in.close();
		ln.closeFile(FILE_ABC);
		System.out.printf("Read from %s: %s", FILE_ABC, new String(b));
	}
}
