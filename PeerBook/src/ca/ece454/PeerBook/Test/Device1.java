package ca.ece454.PeerBook.Test;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.security.NoSuchAlgorithmException;

import ca.ece454.PeerBook.LocalNode;

public class Device1 {
	private static final String f11 = "files\\f11.txt";
	private static final String f12 = "files\\f12.docx";
	private static final String f13 = "files\\f13.pptx";
	private static final String f14 = "files\\f14.txt";
	
	private static LocalNode ln = null;
	
	public static void main(String[] args) throws IOException, NoSuchAlgorithmException {
		System.out.println("========== Device 1 ==========");
		
		int port = Integer.parseInt(args[0]);
		String nodesListFile = args[1]; 
		
		ln = new LocalNode(port, false);
		
		// 0. Add files
		System.out.println("0. Add files...");
		ln.addFile(f11);
		ln.addFile(f12);
		ln.addFile(f13);		
		ln.listFiles();
		
		// 1. Tag file
		System.out.println("1. Tagg files...");
		ln.tagFile(f11);
		ln.tagFile(f12);
		ln.listFiles();
		
		// Join network
		ln.join(nodesListFile);
		
		waitForInput();
		ln.listFiles();
		
		// 4. Leave network
		System.out.println("4. Leave network...");
		waitForInput();
		ln.leave();
		
		// 4. Reconnect to network
		System.out.println("4. Reconnect to network...");
		waitForInput();
		ln.join("nodes_list1.txt");
		
		// 5. Leave network and update f11
		System.out.println("5. Leave network and update f11...");
		waitForInput();
		ln.leave();
		
		BufferedOutputStream out = ln.writeFile(f11);
		out.write(("DEVICE 1!!!!").getBytes());
		out.close();
		ln.closeFile(f11);
		
		// 5. Rejoin network
		System.out.println("5. Rejoin network...");
		waitForInput();		
		ln.join(nodesListFile);
		
		// 6a. Add file f14
		System.out.println("6a. Add file f14...");
		waitForInput();
		ln.addFile(f14);
		
		// 6b. Delete f11
		System.out.println("6b. Delete file f11...");
		waitForInput();
		ln.deleteFile(f11);
		
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
