#include "file_hasher.h"

#include <exception>
#include <algorithm>
#include <boost/container_hash/hash.hpp>

using namespace std;

Hash FileHasher::getNextBlock() {
    if (is_end) {
        throw runtime_error("End of file");
    }
    if (!in_) {
        in_.open(filename_, ios::binary);
    }
    if (!in_) {
        throw invalid_argument("Can't open file " + filename_);
    }

    char* block = new char[block_size_];
    fill(block, block + block_size_, 0);

    if (in_.read(block, block_size_)) {
        //return boost::hash<char*>{}(block);
    }
    return 0;
}
