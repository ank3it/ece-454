package ca.ece454.PeerBook;

import java.util.UUID;

public interface OnReceiveListener {
	/**
	 * Callback to call every time a node receives a message.
	 * 
	 * @param nodeId
	 *            The ID of the node that received the message.
	 * @param message
	 *            The received message.
	 */
	public void onReceive(UUID nodeId, Message message);
}
