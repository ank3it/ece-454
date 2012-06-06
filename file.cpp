#include "file.h"
#include "constants.h"

/*
 * Constructor. Creates an object with an invalid but consistent state
 */
File::File() : _filename(""), _totalChunks(0), _isChunkAvailable(0) {
	// Empty
}

/*
 * Constructor. Initializes the file object to the given value
 *
 * fn: The filename of the file
 * tc: The total number of chunks in the file
 * isAvailable: Whether the file is available locally (i.e. if we have any
 * chunks of the file available or not)
 */
File::File(std::string fn, int tc, bool isAvailable) 
: _filename(fn), _totalChunks(tc) {
	_isChunkAvailable = new bool[_totalChunks];

	if (isAvailable) {		// File available locally
		for (int i = 0; i < _totalChunks; i++)
			_isChunkAvailable[i] = true;
	} else {				// No chunks available locally
		for (int i = 0; i < _totalChunks; i++)
			_isChunkAvailable[i] = false;
	}
}

/*
 * Destructor
 */
File::~File() {
	delete[] _isChunkAvailable;
}

bool File::open() {
	return false;
}

void File::close() {

}

/*
 * Read in the specified chunk into the given buffer. Returns true if something
 * was read.
 *
 * chunkIndex: Zero-based index of the chunk to be read
 * buffer: The buffer in which the chunk data will be returned
 * size: The size of the data to be read
 */
bool File::readChunk(int chunkIndex, char* buffer, int size) {
	if (chunkIndex >= _totalChunks)
		return false;

	_file.open(_filename.c_str(), 
		std::ios::out | std::ios::binary | std::ios::app);

	if (!_file.is_open())
		return false;

	int offset = chunkIndex * constants::CHUNK_SIZE;
	_file.seekg(offset);

	_file.read(buffer, size);
	_file.close();

	return true;
}

/*
 * Write the buffer to the specified chunk. Returns true if something was 
 * written.
 *
 * chunkIndex: Zero-based index of the chunk to be written
 * buffer: The buffer that contains the chunk data
 * size: The size of the data to be written
 */
bool File::writeChunk(int chunkIndex, char* buffer, int size) {
	if (chunkIndex >= _totalChunks)
		return false;

	_file.open(_filename.c_str(), 
		std::ios::in | std::ios::binary | std::ios::app);

	if (!_file.is_open())
		return false;

	int offset = chunkIndex * constants::CHUNK_SIZE;
	_file.seekp(offset);

	_file.write(buffer, size);
	_file.close();

	return true;
}
