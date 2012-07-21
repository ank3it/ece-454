package ca.ece454.PeerBook.Test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import ca.ece454.PeerBook.LocalNode;

public class Test2 {
	public static void main(String[] args) throws IOException {
		System.out.println("========== Test2 ==========");
		
		LocalNode ln = new LocalNode(8081, false);
		ln.startServer();
		ln.join();
		
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
