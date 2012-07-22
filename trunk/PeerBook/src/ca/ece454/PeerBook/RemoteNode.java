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
	private ObjectOutputStream out = null;
	private ObjectInputStream in = null;
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
		log.info("Sending " + message.getMessageType() + " message");
		
		if (!socket.isConnected() || socket.isClosed()) {
			log.warning("Socket closed, unable to send message");
			return;
		}
		
		if (out == null)
			out = new ObjectOutputStream(socket.getOutputStream());
		
		out.writeObject(message);
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
		
		try {
			if (in == null)
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
			e.printStackTrace();
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
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
