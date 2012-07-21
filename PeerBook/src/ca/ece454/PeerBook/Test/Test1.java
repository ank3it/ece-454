package ca.ece454.PeerBook.Test;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.security.NoSuchAlgorithmException;
import java.util.Iterator;

import ca.ece454.PeerBook.FileMetadata;
import ca.ece454.PeerBook.Message;
import ca.ece454.PeerBook.Message.MessageType;
import ca.ece454.PeerBook.PeerBookFile;
import ca.ece454.PeerBook.Util.Util;

public class Test1 {
	public static void main(String[] args) throws IOException, NoSuchAlgorithmException {
		System.out.println("========== Test1 ==========");
		
//		LocalNode ln = new LocalNode(8080);
//		ln.startServer();
//		
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
		PeerBookFile file = new PeerBookFile("test.jpg", ".", true, true, true, false);
		file.tag();
		file.updateChecksum(Util.CHECKSUM_ALGORITHM);
//		FileSystemFile file2 = new FileSystemFile("rubric.txt", ".", true, true, true);
//		file2.updateChecksum(Util.CHECKSUM_ALGORITHM);
		
		Message msg1 = new Message(MessageType.DOWNLOAD_REQUEST, false);
		msg1.addFileMetadata(file.getFileMetadata());
		msg1.setFileData(file.read());
		
		for (Iterator<FileMetadata> it = msg1.getFilesListIterator(); it.hasNext();) {
			System.out.println(Util.checksumString(it.next().getChecksum()));
		}
		
		FileOutputStream fos;
		ObjectOutputStream out;
		
		// Serialize
		try {
			fos = new FileOutputStream("test.txt");
			out = new ObjectOutputStream(fos);
			out.writeObject(msg1);
			out.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		// Deserialize
		Message msg2;
		FileInputStream fis;
		ObjectInputStream in;
		
		try {
			fis = new FileInputStream("test.txt");
			in = new ObjectInputStream(fis);
			msg2 = (Message) in.readObject();
			
			PeerBookFile f = new PeerBookFile(msg2.getFileMetadata(0));
			f.getFileMetadata().setFilename("helloooooooo.jpg");
			f.write(msg2.getFileData());
			
			for (Iterator<FileMetadata> it = msg2.getFilesListIterator(); it.hasNext();) {
				System.out.println(Util.checksumString(it.next().getChecksum()));
			}
			
			in.close();
		} catch (IOException e) {
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
		}
		
		System.out.println("exiting main()");
	}
}
