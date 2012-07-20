package ca.ece454.PeerBook;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.logging.Logger;

public class Server implements Runnable {
	private static final Logger log = Logger.getLogger(Server.class.getName());
	private ServerSocket serverSocket;
	private int port;
	private OnReceiveListener onReceiveListener;

	/**
	 * Creates a server responsible for listening for connecting nodes.
	 * 
	 * @param port
	 *            The port number to bind the server to.
	 * @param onReceiveListener
	 *            The OnReceiveListener which is assigned to each connecting
	 *            node. This callback is called every time the node receives a
	 *            message.
	 */
	public Server(int port, OnReceiveListener onReceiveListener) {
		this.port = port;
		this.onReceiveListener = onReceiveListener;
	}

	/**
	 * Server is started, allowing it to receive connections. Listening for new
	 * connections is performed in new thread.
	 * 
	 * @throws IOException
	 */
	public void start() throws IOException {
		if (serverSocket == null || serverSocket.isClosed()) {
			serverSocket = new ServerSocket(port);
			(new Thread(this)).start();
		}
	}

	/**
	 * Stops the server and closes the socket used by the server for listening
	 * and accepting connections.
	 * 
	 * @throws IOException
	 */
	public void stop() throws IOException {
		if (serverSocket == null || serverSocket.isClosed())
			return;

		serverSocket.close();
	}

	@Override
	public void run() {
		log.info("Started server thread");

		while (!serverSocket.isClosed()) {
			Socket clientSocket;
			try {
				// Accept connections from other nodes
				clientSocket = serverSocket.accept();
				clientSocket.setKeepAlive(true);

				log.info(clientSocket.toString() + " connected");

				// Create RemoteNode object and start its associated thread
				RemoteNode node = new RemoteNode(clientSocket);
				node.setOnReceiveListener(onReceiveListener);
				NodeManager.getInstance().addNode(node);
				(new Thread(node)).start();
			} catch (IOException e) {
				log.severe(e.toString());
			}
		}
	}
}
