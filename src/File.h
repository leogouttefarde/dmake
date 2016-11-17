
#pragma once
#include <iostream>
#include <string>
#include <pup_stl.h>


class File {

public:
	std::string mPath;

	File(std::string path, bool preload = true) : mPath(path) {

		if (preload)
			read();
	}

	File() {
	}

	~File() {
	}

	void pup(PUP::er &p) {
		p|mPath;
		p|mData;
	}

	inline File &operator=(const File &infile) {
		mPath = infile.mPath;
		mData = infile.mData;

		return *this;
	}

	// Read file data from path
	void read() {

		FILE *file = fopen( mPath.c_str(), "rb" );

		if ( file != NULL ) {

			// Compute file size
			fseek(file, 0, SEEK_END);
			unsigned long size = ftell(file);
			fseek(file, 0, SEEK_SET);

			// Allocate file space
			mData.resize( size );

			// Read file data
			fread((void*)mData.data(), 1, size, file);
			fclose(file);
		}
	}

	void write() {

		if (!mData.size())
			return;

		// Forcefully write file into path
		FILE *file = fopen( mPath.c_str(), "wb" );

		if ( file != NULL ) {
			fwrite(mData.data(), 1, mData.size(), file);
			fclose(file);
		}

	}

	uint64_t size() {
		return mData.size();
	}

	// A way to store binary data manually (for test purposes)
	void setData( uint8_t *data, uint64_t size ) {

		if (data == NULL || size == 0) {
			return;
		}

		mData.resize(size);

		for (uint64_t i = 0; i < size; i++) {
			mData[i] = data[i];
		}
	}


private:
	std::vector<uint8_t> mData;

};


