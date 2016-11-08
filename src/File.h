
#pragma once
#include <string>
#include <pup_stl.h>


class File {

public:

  File(std::string path, bool preload = true) : mPath(path) {
    mData = NULL;
    mSize = 0;

    if (preload)
      read();
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

    if (mData != NULL)
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

    // force write file into path (do not rely on NFS)
    std::string path(mPath);

    // path += std::to_string(index) + ".txt";

    FILE *file = fopen( path.c_str(), "rb" );

    if ( file != NULL ) {
      fseek(file, 0, SEEK_END);
      mSize = ftell(file);
      fseek(file, 0, SEEK_SET);

      mData = new uint8_t[mSize];

      if (mData) {

        // assert( mSize == fwrite(mData, 1, mSize, file) );
        fread(mData, 1, mSize, file);
      }

      fclose(file);
    }
  }

  void write(/* int index*/ ) {

    if (!mData || !mSize)
      return;

    // force write file into path (do not rely on NFS)
    std::string path(mPath);

    // path += std::to_string(index) + ".txt";

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


