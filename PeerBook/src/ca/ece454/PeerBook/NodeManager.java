package ca.ece454.PeerBook;

import java.io.FileInputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.net.UnknownHostException;
import java.util.AbstractMap.SimpleEntry;
import java.util.Scanner;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;
import java.util.logging.Logger;

public class NodeManager {
	private static final Logger log = Logger.getLogger(NodeManager.class
			.getName());
	private static NodeManager instance;
	private ConcurrentHashMap<UUID, RemoteNode> nodesTable;

	/**
	 * Private constructor.
	 */
	private NodeManager() {
		this.nodesTable = new ConcurrentHashMap<UUID, RemoteNode>();
	}

	/**
	 * Returns an instance of the NodeManager class.
	 * 
	 * @return An instance of the NodeManager class.
	 */
	public static synchronized NodeManager getInstance() {
		if (instance == null)
			instance = new NodeManager();
		return instance;
	}
	
	/**
	 * Returns true if the node manager has at least one node (i.e. It is
	 * connected to at least one node).
	 * 
	 * @return True if it is connected.
	 */
	public boolean isConnected() {
		return nodesTable.size() > 0;
	}

	/**
	 * Attempt to connect to each node in the given file which contains the ip
	 * address and port number to connect to. The format of the file is:
	 * "[ip address] [port number]"
	 * 
	 * @param nodesListFilepath
	 *            The filepath to the file containing the list of nodes.
	 * @throws IOException
	 * @throws UnknownHostException
	 */
	public void connectToNodes(String nodesListFilepath, OnReceiveListener onReceiveListener)
			throws UnknownHostException, IOException {
		log.info("Attempting to connect to network");

		Scanner scanner = new Scanner(new FileInputStream(nodesListFilepath));
		try {
			while (scanner.hasNextLine()) {
				String host = scanner.next();
				int port = scanner.nextInt();

				log.info("Connecting to " + host + ":" + port);

				Socket socket = new Socket();
				SocketAddress endpoint = new InetSocketAddress(host, port);
				socket.connect(endpoint, 2000);
				socket.setKeepAlive(true);
				if (socket.isConnected()) {
					RemoteNode node = new RemoteNode(socket);
					node.setOnReceiveListener(onReceiveListener);
					addNode(node);
					(new Thread(node)).start();
				}
			}
		} finally {
			scanner.close();
		}
	}

	/**
	 * Disconnect from all nodes and clear the table of nodes.
	 */
	public void disconnectFromNodes() {
		for (RemoteNode node : nodesTable.values()) {
			try {
				node.disconnect();
			} catch (IOException e) {
				log.severe(e.toString());
			}
		}

		nodesTable.clear();
	}

	/**
	 * Broadcasts the given message to all nodes.
	 * 
	 * @param message
	 *            The message to be broadcast.
	 * @throws IOException 
	 */
	public void broadcastMessage(Message message) throws IOException {
		for (RemoteNode node : nodesTable.values()) {
			node.sendMessage(message);
		}
	}

	/**
	 * Rebroadcasts the message to all nodes except the message sender. UUID is
	 * used to determine who is the message sender.
	 * 
	 * @param entry
	 *            A SimpleEntry object containing the UUID of the sender and the
	 *            message.
	 * @throws IOException
	 */
	public void reBroadcastMessage(SimpleEntry<UUID, Message> entry) throws IOException {
		for (RemoteNode node : nodesTable.values()) {
			if (node.getNodeId().equals(entry.getKey()))
				continue;

			node.sendMessage(entry.getValue());
		}
	}

	/**
	 * Add the given node to the map of managed nodes. Returns false if the node
	 * is already managed.
	 * 
	 * @param node
	 *            The node to be added.
	 */
	public void addNode(RemoteNode node) {
		if (node == null)
			return;

		nodesTable.put(node.getNodeId(), node);
	}

	/**
	 * Removes the node from the map of managed nodes.
	 * 
	 * @param node
	 *            The node to be removed.
	 */
	public void removeNode(RemoteNode node) {
		if (node == null)
			return;

		nodesTable.remove(node);
	}

	/**
	 * Returns the RemoteNode object with the given node ID.
	 * 
	 * @param nodeID
	 *            The ID of the node to retrieve.
	 * @return The node if it exists, null otherwise.
	 */
	public RemoteNode getNode(UUID nodeID) {
		return nodesTable.get(nodeID);
	}

	/**
	 * Returns a node maintained by the node manager. Successive calls may
	 * return different nodes.
	 * @return A RemoteNode currently managed by the NodeManager.
	 */
	public RemoteNode getNode() {
		return (RemoteNode) nodesTable.values().toArray()[0];
	}
}