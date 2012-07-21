package ca.ece454.PeerBook.Test;

import java.io.IOException;
import java.security.NoSuchAlgorithmException;

import ca.ece454.PeerBook.LocalNode;

public class Test3 {
	public static void main(String[] args) throws IOException, NoSuchAlgorithmException {
		System.out.println("========== Test3 ==========");
		
		LocalNode ln = new LocalNode(8080, false);
//		ln.startServer();
		ln.addFile("test.jpg");
		ln.addFile("rubric.txt");
		ln.addFile("Todo.txt");
		ln.tagFile("test.jpg");
		ln.listFiles();
		ln.tagFile("test.jpg");
		
		ln.listFileVersions("test.jpg");
		
		
		
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
}
