package ca.ece454.PeerBook.Test;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import ca.ece454.PeerBook.LocalNode;


public class SystemAPITest {
	private static final String f11 = "files\\f11.txt";
	private static final String f12 = "files\\f12.docx";
	private static final String f13 = "files\\f13.pptx";

	public static void main(String[] args) throws IOException {
		LocalNode ln = new LocalNode(8080, false);
		
		// Add exiting files
		ln.addFile(f12);
		ln.addFile(f13);
		
		// Create new file
		BufferedOutputStream out = ln.createFile(f11);
		String str = "This file was created by SystemAPITest!";
		out.write(str.getBytes());
		out.close();
		ln.closeFile(f11);
		
		// List files
		ln.listFiles();
		
		System.out.println("Deleting " + f13);
		waitForInput();
		
		// Delete "test - Copy.jpg"
		ln.deleteFile(f13);
		
		// List files
		ln.listFiles();		
		
		System.out.println("Tagging " + f11 + " as stable version");
		waitForInput();
		
		// Tag file abc.txt
		ln.tagFile(f11);
		
		// List versions for abc.txt
		ln.listFileVersions(f11);
		
		System.out.println("Writing to " + f11);
		waitForInput();
		
		// Write to abc.txt
		out = ln.writeFile(f11);
		out.write(("Hello!").getBytes());
		out.close();
		ln.closeFile(f11);
		
		// Read versioned file
		BufferedInputStream in = ln.readFile(f11, 1);
		byte[] a = new byte[100];
		in.read(a, 0, 100);
		in.close();
		ln.closeFile(f11);
		System.out.printf("Read from %s version %s: %s%n", f11, 1, new String(a));
		
		System.out.println("Tagging " + f11 + " as stable version again");
		waitForInput();
		
		// Tag abc.txt again
		ln.tagFile(f11);
		
		// List files
		ln.listFiles();
		
		System.out.println("Reading from " + f11);
		waitForInput();
		
		// Reading from abc.txt
		in = ln.readFile(f11);
		byte[] b = new byte[100];
		in.read(b, 0, 100);
		in.close();
		ln.closeFile(f11);
		System.out.printf("Read from %s: %s%n", f11, new String(b));
	}
	
	private static void waitForInput() throws IOException {
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		reader.read();
	}
}
