
#pragma once
#include <iostream>
#include <string>
#include <pup_stl.h>


class File {

public:

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

	// read file data from path
	void read() {

		// force write file into path (do not rely on NFS)
		std::string path(mPath);

		FILE *file = fopen( path.c_str(), "rb" );

		if ( file != NULL ) {
			fseek(file, 0, SEEK_END);
			unsigned long size = ftell(file);
			fseek(file, 0, SEEK_SET);

			mData.resize( size );

			// assert( mSize == fwrite(mData, 1, mSize, file) );
			fread((void*)mData.data(), 1, size, file);

			fclose(file);
		}
	}

	void write() {

		if (!mData.size())
			return;

		// force write file into path (do not rely on NFS)
		std::string path(mPath);

		FILE *file = fopen( path.c_str(), "wb" );

		if ( file != NULL ) {
			// assert( mSize == fwrite(mData, 1, mSize, file) );
			fwrite(mData.data(), 1, mData.size(), file);
			fclose(file);
		}

	}

	uint64_t size() {
		return mData.size();
	}

	void setData( uint8_t *data, uint64_t size ) {

		if (data == NULL || size == 0) {
			return;
		}

		mData.resize(size);

		for (uint64_t i = 0; i < size; i++) {
			mData[i] = data[i];
		}
	}

	std::string mPath;

private:
	std::vector<uint8_t> mData;

};


