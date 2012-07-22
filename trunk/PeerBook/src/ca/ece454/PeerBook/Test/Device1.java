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
	
	public static void main(String[] args) throws IOException, NoSuchAlgorithmException {
		System.out.println("========== Device 1 ==========");
		
		LocalNode ln = new LocalNode(8080, false);
		
		// 0. Add files
		System.out.println("0. Adding files...");
		ln.addFile(f11);
		ln.addFile(f12);
		ln.addFile(f13);		
		ln.listFiles();
		
		// 1. Tag file
		System.out.println("1. Tagging files...");
		ln.tagFile(f12);
		ln.listFiles();
		
		// Join network
		ln.join("nodes_list1.txt");
		
		waitForInput();
		ln.listFiles();
		
		System.out.println("4. Leaving network...");
		waitForInput();
		
		// 4. Leave network
		ln.leave();
		
		System.out.println("4. Reconnect to network...");
		waitForInput();
		
		// 4. Reconnect to network
		ln.join("nodes_list1.txt");
		
		System.out.println("5. Leaving network and updating f11...");
		waitForInput();
		
		// 5. Leave network and update f11
		ln.leave();
		
		System.out.println("Writing to file...");
		BufferedOutputStream out = ln.writeFile(f11);
		out.write(("DEVICE 1!!!!").getBytes());
		out.close();
		ln.closeFile(f11);
		
		System.out.println("6a. Adding file f14...");
		waitForInput();
		
		// 6a. Add file f14
		ln.addFile(f14);
		
		System.out.println("6b. Deleting file f11...");
		waitForInput();
		
		// 6b. Delete f11
		ln.deleteFile(f11);
		
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
		
		// Create and initialize file object
//		PeerBookFile file = new PeerBookFile("test.jpg", ".", true, true, true, false);
//		file.tag();
//		file.updateChecksum(Util.CHECKSUM_ALGORITHM);
////		FileSystemFile file2 = new FileSystemFile("rubric.txt", ".", true, true, true);
////		file2.updateChecksum(Util.CHECKSUM_ALGORITHM);
//		
//		Message msg1 = new Message(MessageType.DOWNLOAD_REQUEST, false);
//		msg1.addFileMetadata(file.getFileMetadata());
//		msg1.setFileData(file.read());
//		
//		for (Iterator<FileMetadata> it = msg1.getFilesListIterator(); it.hasNext();) {
//			System.out.println(Util.checksumString(it.next().getChecksum()));
//		}
//		
//		FileOutputStream fos;
//		ObjectOutputStream out;
//		
//		// Serialize
//		try {
//			fos = new FileOutputStream("test.txt");
//			out = new ObjectOutputStream(fos);
//			out.writeObject(msg1);
//			out.close();
//		} catch (IOException e) {
//			e.printStackTrace();
//		}
//		
//		// Deserialize
//		Message msg2;
//		FileInputStream fis;
//		ObjectInputStream in;
//		
//		try {
//			fis = new FileInputStream("test.txt");
//			in = new ObjectInputStream(fis);
//			msg2 = (Message) in.readObject();
//			
//			PeerBookFile f = new PeerBookFile(msg2.getFileMetadata(0));
//			f.getFileMetadata().setFilename("helloooooooo.jpg");
//			f.write(msg2.getFileData());
//			
//			for (Iterator<FileMetadata> it = msg2.getFilesListIterator(); it.hasNext();) {
//				System.out.println(Util.checksumString(it.next().getChecksum()));
//			}
//			
//			in.close();
//		} catch (IOException e) {
//			e.printStackTrace();
//		} catch (ClassNotFoundException e) {
//			e.printStackTrace();
//		}
		
		System.out.println("exiting main()");
	}
	
	private static void waitForInput() throws IOException {
		System.out.println("Press enter to continue...");
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		reader.read();
	}
}
