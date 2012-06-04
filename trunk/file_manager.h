#pragma once
#include "local_peer.h"
#include "file_chunk.h"


class file_manager
{
public:
	file_manager(void);
	~file_manager(void);
	int chunk_file(std::string filename);
};

