package ca.ece454.PeerBook;

public class UserTag {
	private final String filepath;
	private final int version;
	private final long tagDate;

	/**
	 * Constructor
	 * @param filepath The filepath of the versioned file.
	 * @param version The version of the file.
	 * @param tagDate The data in number of milliseconds since the Unix epoch.
	 */
	public UserTag(String filepath, int version, long tagDate) {
		this.filepath = filepath;
		this.version = version;
		this.tagDate = tagDate;
	}

	public String getFilepath() {
		return filepath;
	}

	public int getVersion() {
		return version;
	}

	public long getTagDate() {
		return tagDate;
	}
}
