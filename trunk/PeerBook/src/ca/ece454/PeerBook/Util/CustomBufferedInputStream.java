package ca.ece454.PeerBook.Util;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;

import ca.ece454.PeerBook.LocalNode;

public class CustomBufferedInputStream extends BufferedInputStream {
	private final LocalNode localNode;

	public CustomBufferedInputStream(InputStream inputStream, LocalNode localNode) {
		super(inputStream);
		this.localNode = localNode;
	}
	
	@Override
	public void close() throws IOException {
		super.close();
	}
}
