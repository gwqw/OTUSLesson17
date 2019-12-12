#include "file_hasher.h"

using namespace std;

Hash FileHasher::getNext(std::ifstream& in) {
    Block b(block_size_);
    std::fill(b.begin(), b.end(), 0);
    std::size_t readed = 0;
    while (in && readed < block_size_) {
        in.read(b.data() + readed, 1);
        ++readed;
    }
    return hasher_(b);
}

std::optional<Hash> FileHasher::operator[](std::size_t idx) {
    if (idx < blocks_cache_.size()) {
        return blocks_cache_[idx];
    }
    if (idx * block_size_>= getFileSize()) {
        return nullopt;
    }

    std::ifstream in;
    in.rdbuf()->pubsetbuf(nullptr, 0);
    in.open(filename_, std::ios::binary|std::ios_base::in);
    in.seekg(blocks_cache_.size() * block_size_);
    if (!in) {
        return nullopt;
    }

    blocks_cache_.reserve(idx + 1);
    while (blocks_cache_.size() <= idx && in) {
        blocks_cache_.push_back(getNext(in));
    }

    if (idx < blocks_cache_.size()) {
        return blocks_cache_[idx];
    } else {
        return nullopt;
    }
}

std::size_t FileHasher::getFileSize() {
    if (file_size_) {
        return *file_size_;
    }
    std::ifstream in;
    in.rdbuf()->pubsetbuf(nullptr, 0);
    in.open(filename_, std::ios::binary|std::ios_base::in);
    in.seekg(0, std::ios::end);
    file_size_ = in.tellg();
    return *file_size_;
}

bool operator==(FileHasher &lhs, FileHasher &rhs) {
    if (lhs.getFileSize() != rhs.getFileSize()) {
        return false;
    }
    bool res = true;
    for (size_t i = 0; lhs[i].has_value() && rhs[i].has_value(); ++i) {
        if (*lhs[i] != *rhs[i]) {
            res = false;
            break;
        }
    }
    return res;
}
