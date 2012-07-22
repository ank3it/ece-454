package ca.ece454.PeerBook.Test;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.security.NoSuchAlgorithmException;

import ca.ece454.PeerBook.LocalNode;

public class Device3 {
	private static final String f11 = "files\\f11.txt";
	private static final String f12 = "files\\f12.docx";
	private static final String f31 = "files\\f31.jpg";
	
	private static LocalNode ln = null;
	
	public static void main(String[] args) throws IOException, NoSuchAlgorithmException {
		System.out.println("========== Device 3 ==========");
		
		int port = Integer.parseInt(args[0]);
		String nodesListFile = args[1]; 
		
		ln = new LocalNode(port, true);
		
		// Add files
		ln.addFile(f31);
		ln.listFiles();
		
		// Join network
		System.out.println("Join network...");
		waitForInput();
		ln.join(nodesListFile);
		
		// List files
		System.out.println("List files...");
		waitForInput();		
		ln.listFiles();
		ln.listFileVersions(f12);
		
		// Read file
		System.out.println("Read file f11...");
		waitForInput();
		
		BufferedInputStream in = ln.readFile(f11);
		byte[] a = new byte[100];
		in.read(a, 0, 100);
		in.close();
		ln.closeFile(f11);
		System.out.printf("Read from %s version %s: %s%n", f11, 1, new String(a));
		
		// Read versioned file
		System.out.println("Reading file f12 version 1");
		waitForInput();
		in = ln.readFile(f12, 1);
		in.read(a, 0, 100);
		in.close();
		
		// List files
		System.out.println("List files...");
		waitForInput();		
		ln.listFiles();
		
		// Exit
		System.out.println("Exit...");
		waitForInput();
		ln.leave();
		System.exit(0);
	}
	
	private static void waitForInput() throws IOException {
		System.out.println("Press enter to continue...");
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		String line = reader.readLine();
		if (line.equalsIgnoreCase("q")) {
			ln.leave();
			System.exit(0);
		}
	}
}
