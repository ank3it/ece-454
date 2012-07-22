package ca.ece454.PeerBook.Test;

import ca.ece454.PeerBook.LocalNode;

public class Test2 {

	public static void main(String[] args) {
		LocalNode ln = new LocalNode(8082, false);
		ln.join("nodes_list2.txt");
		
		while (true);
	}

}
