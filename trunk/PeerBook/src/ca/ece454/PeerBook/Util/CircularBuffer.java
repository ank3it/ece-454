package ca.ece454.PeerBook.Util;

/**
 * Circular buffer that overrides previous entries on add. This class only
 * provides the ability to add items and check whether a given item exists in
 * the circular buffer.
 * 
 * @param <T>
 */
public class CircularBuffer<T> {
	private T[] buffer;
	private int index;

	/**
	 * Constructor
	 * @param size The size of the buffer to create.
	 */
	@SuppressWarnings("unchecked")
	public CircularBuffer(int size) {
		this.buffer = (T[]) new Object[size];
		this.index = 0;
	}

	/**
	 * Adds the given item to the circular buffer. May override existing
	 * entries.
	 * 
	 * @param item
	 *            The item to be added to the circular buffer.
	 */
	public void add(T item) {
		buffer[index++] = item;
		index %= buffer.length;
	}

	/**
	 * Returns true if the circular buffer contains the specified item.
	 * 
	 * @param item
	 *            The item to look for.
	 * @return True if the item is found in the circular buffer.
	 */
	public boolean contains(T item) {
		for (int i = 0; i < index; i++) {
			if (buffer[i].equals(item))
				return true;
		}

		return false;
	}
}
