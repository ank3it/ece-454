package ca.ece454.PeerBook;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.UUID;
import java.util.logging.Logger;

public class RemoteNode implements Runnable {
	private static final Logger log = Logger.getLogger(RemoteNode.class.getName());

	private final UUID nodeId;
	private Socket socket;
	private OnReceiveListener onReceiveListener;

	public RemoteNode(Socket socket) {
		this.nodeId = UUID.randomUUID();
		this.socket = socket;
	}	

	/**
	 * Send the given message to the remote node. This method is synchronized.
	 * 
	 * @param message
	 *            The Message object to be sent.
	 * @throws IOException
	 */
	public synchronized void sendMessage(Message message) throws IOException {
		ObjectOutputStream out = null;

		try {
			out = new ObjectOutputStream(socket.getOutputStream());
			out.writeObject(message);
		} finally {
			if (out != null)
				out.close();
		}
	}
	
	/**
	 * Closes the connection with the node.
	 * @throws IOException
	 */
	public void disconnect() throws IOException {
		if (socket != null)
			socket.close();
	}

	@Override
	public void run() {
		log.info("Started RemoteNode thread");
		ObjectInputStream in = null;
		
		try {
			in = new ObjectInputStream(socket.getInputStream());
			
			try {
				while (socket.isConnected() && !socket.isClosed() && !socket.isInputShutdown()) {
					Message message = (Message) in.readObject();
				
					if (onReceiveListener != null && message != null)
						onReceiveListener.onReceive(nodeId, message);
				}
			} finally {
				in.close();
			}
		} catch (IOException e) {
			log.severe(e.toString());
		} catch (ClassNotFoundException e) {
			log.severe(e.toString());
		}
	}

	// Getters / setters
	public UUID getNodeId() {
		return nodeId;
	}

	public void setOnReceiveListener(OnReceiveListener onReceiveListener) {
		this.onReceiveListener = onReceiveListener;
	}
}
