
#pragma once
#include <string>
#include <pup_stl.h>


class File {

public:

  File(std::string path) : mPath(path) {
    mData = NULL;
    mSize = 0;
  }

  File() {
    mData = NULL;
    mSize = 0;
  }

  ~File() {
    if (mData != NULL) {
      delete [] mData;
    }
  }

  void pup(PUP::er &p) {
    // remember to pup your superclass if there is one
    p|mPath;
    p|mSize;

    if (p.isUnpacking())
      mData = new uint8_t[mSize];

    PUParray(p, mData, mSize);
  }

  inline File &operator=(const File &infile) {

    if (mData != NULL)
      delete [] mData;

    mSize = infile.mSize;
    mPath = infile.mPath;

    if (mSize > 0)
      mData = new uint8_t[mSize];

    for (uint64_t i = 0; i < mSize; ++i)
      mData[i] = infile.mData[i];

    return *this;
  }

  void read() {
    // read file data from path
  }

  void write( int index ) {
    // force write file into path (do not rely on NFS)
    std::string path(mPath);

    path += std::to_string(index) + ".txt";

    FILE *file = fopen( path.c_str(), "wb" );

    if ( file != NULL ) {
      // assert( mSize == fwrite(mData, 1, mSize, file) );
      fwrite(mData, 1, mSize, file);
      fclose(file);
    }
  }

  uint64_t size() {
    return mSize;
  }

  void setData( uint8_t *data, uint64_t size ) {

    if (data == NULL || size == 0) {
      return;
    }

    mData = new uint8_t[size];
    mSize = size;

    for (uint64_t i = 0; i < mSize; i++) {
      mData[i] = data[i];
    }
  }


private:
  uint64_t mSize;
  uint8_t *mData;

  std::string mPath;

};


