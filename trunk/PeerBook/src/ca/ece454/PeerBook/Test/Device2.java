package ca.ece454.PeerBook.Test;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import ca.ece454.PeerBook.LocalNode;

public class Device2 {
	private static final String f11 = "files\\f11.txt";
	private static final String f21 = "files\\f21.exe";
	private static final String f22 = "files\\f22.zip";
	
	public static void main(String[] args) throws IOException {
		System.out.println("========== Device 2 ==========");
		
		LocalNode ln = new LocalNode(8080, false);
		
		// 0. Add files
		System.out.println("0. Adding files...");
		ln.addFile(f21);
		ln.addFile(f22);		
		ln.listFiles();		
		
		// 1. Tag file
		System.out.println("1. Tagging files...");
		ln.tagFile(f21);
		ln.listFiles();
		
		// Join network
		ln.join("nodes_list2.txt");
		
		waitForInput();
		ln.listFiles();
		
		System.out.println("3. Reading file f11...");
		waitForInput();
		
		// 3. Read file
		BufferedInputStream in = ln.readFile(f11);
		byte[] b = new byte[100];
		in.read(b, 0, 100);
		in.close();
		ln.closeFile(f11);
		System.out.printf("Read from %s: %s%n", f11, new String(b));
		
		System.out.println("Writing to file f11...");
		waitForInput();
		
		// 4. Write to file
		BufferedOutputStream out = ln.writeFile(f11);
		out.write(("This file was updated by DEVICE 2!").getBytes());
		out.close();
		ln.closeFile(f11);
		
		System.out.println("5. Leaving network and writing to f11...");
		waitForInput();
		
		// 5. Leave network and update f11
		ln.leave();
		
		out = ln.writeFile(f11);
		out.write(("Another update by DEVICE 2!").getBytes());
		out.close();
		ln.closeFile(f11);
		
		System.out.println("6a,b. Printing list of files...");
		waitForInput();
		
		// 6a,b. Print list of files
		ln.listFiles();
		
		waitForInput();		
		
//		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
//		String line;
//		while ((line = reader.readLine()) != null) {
//			if (line.equalsIgnoreCase("q")) {
//				ln.stopServer();
//				ln.leave();
//				System.exit(0);
//			}
//		}
	}
	
	private static void waitForInput() throws IOException {
		System.out.println("Press enter to continue...");
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		reader.read();
	}
}
