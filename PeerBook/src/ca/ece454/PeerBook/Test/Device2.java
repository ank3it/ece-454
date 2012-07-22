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
	
	private static LocalNode ln = null;
	
	public static void main(String[] args) throws IOException {
		System.out.println("========== Device 2 ==========");
		
		int port = Integer.parseInt(args[0]);
		String nodesListFile = args[1]; 
		
		ln = new LocalNode(port, false);
		
		// 0. Add files
		System.out.println("0. Add files...");
		ln.addFile(f21);
		ln.addFile(f22);		
		ln.listFiles();		
		
		// 1. Tag file
		System.out.println("1. Tag files...");
		ln.tagFile(f21);
		ln.listFiles();
		
		// Join network
		ln.join(nodesListFile);
		
		waitForInput();
		ln.listFiles();
		
		// 3. Read file
		System.out.println("3. Read file f11...");
		waitForInput();
		
		BufferedInputStream in = ln.readFile(f11);
		byte[] b = new byte[100];
		in.read(b, 0, 100);
		in.close();
		ln.closeFile(f11);
		System.out.printf("Read from %s: %s%n", f11, new String(b));
		
		// 4. Write to file
		System.out.println("Write to file f11...");
		waitForInput();
		BufferedOutputStream out = ln.writeFile(f11);
		out.write(("This file was updated by DEVICE 2!").getBytes());
		out.close();
		ln.closeFile(f11);
		
		// 5. Leave network and update f11
		System.out.println("5. Leave network and write to f11...");
		waitForInput();
		ln.leave();
		
		out = ln.writeFile(f11);
		out.write(("Another update by DEVICE 2!").getBytes());
		out.close();
		ln.closeFile(f11);
		
		// 5. Rejoin network
		ln.join(nodesListFile);
		
		// 6a,b. Print list of files
		System.out.println("6a,b. Print list of files...");
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
