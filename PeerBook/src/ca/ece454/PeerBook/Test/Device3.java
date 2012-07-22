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
	
	public static void main(String[] args) throws IOException, NoSuchAlgorithmException {
		System.out.println("========== Device 3 ==========");
		
		LocalNode ln = new LocalNode(8081, true);
		
		ln.addFile(f31);
		ln.listFiles();
		
		waitForInput();
		
		ln.join("nodes_list3.txt");
		
		waitForInput();
		
		ln.listFiles();
		ln.listFileVersions(f12);
		
		waitForInput();
		
		// Read versioned file
		BufferedInputStream in = ln.readFile(f11);
		byte[] a = new byte[100];
		in.read(a, 0, 100);
		in.close();
		ln.closeFile(f11);
		System.out.printf("Read from %s version %s: %s%n", f11, 1, new String(a));
		
//		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
//		String line;
//		while ((line = reader.readLine()) != null) {
//			if (line.equalsIgnoreCase("q")) {
//				ln.stopServer();
//				ln.leave();
//				System.exit(0);
//			}
//		}
		
		System.out.println("exiting main()");
	}
	
	private static void waitForInput() throws IOException {
		System.out.println("Press enter to continue...");
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		reader.read();
	}
}
