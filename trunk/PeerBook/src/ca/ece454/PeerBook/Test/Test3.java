package ca.ece454.PeerBook.Test;

import ca.ece454.PeerBook.LocalNode;

public class Test3 {

	public static void main(String[] args) {
		LocalNode ln = new LocalNode(8083, false);
		ln.join("nodes_list3.txt");
		
		while (true);
	}

}
