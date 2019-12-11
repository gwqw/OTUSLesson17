#include "file_hasher.h"

using namespace std;

Hash FileHasher::getNext() {
    Block b(block_size_);
    std::fill(b.begin(), b.end(), 0);
    std::size_t readed = 0;
    while (in_ && readed < block_size_) {
        in_.read(b.data() + readed, 1);
        ++readed;
    }
    return hasher_(b);
}

std::optional<Hash> FileHasher::operator[](std::size_t idx) {
    if (idx < blocks_cache_.size()) {
        return blocks_cache_[idx];
    }
    blocks_cache_.reserve(idx + 1);
    while (blocks_cache_.size() <= idx && hasNext()) {
        blocks_cache_.push_back(getNext());
    }

    if (blocks_cache_.size() == idx+1) {
        return blocks_cache_[idx];
    } else {
        return nullopt;
    }
}

bool operator==(FileHasher &lhs, FileHasher &rhs) {
    bool res = true;
    for (size_t i = 0; lhs[i].has_value() && rhs[i].has_value(); ++i) {
        if (*lhs[i] != *rhs[i]) {
            res = false;
            break;
        }
    }
    if (res && lhs.hasNext() != rhs.hasNext()) res = false;
    return res;
}
