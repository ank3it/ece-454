package ca.ece454.PeerBook.Test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import ca.ece454.PeerBook.LocalNode;

public class Test2 {
	private static final String f21 = "files\\f21.exe";
	private static final String f22 = "files\\f22.zip";
	
	public static void main(String[] args) throws IOException {
		System.out.println("========== Test2 ==========");
		
		LocalNode ln = new LocalNode(8080, false);
		
		System.out.println("Adding files...");
		ln.addFile(f21);
		ln.addFile(f22);		
		ln.listFiles();		
		
		System.out.println("Tagging files...");
		ln.tagFile(f21);
		ln.listFiles();
		
		ln.join("nodes_list2.txt");
		
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		String line;
		while ((line = reader.readLine()) != null) {
			if (line.equalsIgnoreCase("q")) {
				ln.stopServer();
				ln.leave();
				System.exit(0);
			}
		}
	}
}
