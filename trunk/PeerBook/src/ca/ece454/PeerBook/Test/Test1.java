package ca.ece454.PeerBook.Test;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import ca.ece454.PeerBook.LocalNode;

public class Test1 {
	private static final String abc = "files\\abc.txt";

	public static void main(String[] args) throws IOException {
		LocalNode ln = new LocalNode(8081, false);
		ln.join("nodes_list1.txt");
		
		waitForInput();
		
		// Create file
		BufferedOutputStream out = ln.createFile(abc);
		String str = "This file was created by SystemAPITest!";
		out.write(str.getBytes());
		out.close();
		ln.closeFile(abc);
		
		waitForInput();
		
		// Write to abc.txt
		out = ln.writeFile(abc);
		out.write(("Hello!").getBytes());
		out.close();
		ln.closeFile(abc);
		
		while (true);
	}
	
	private static void waitForInput() throws IOException {
		System.out.println("Press enter to continue...");
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		reader.readLine();
	}

}
