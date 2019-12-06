#pragma once

#include <string>
#include <vector>
#include <fstream>

using Hash = int;

class FileHasher {
public:
    FileHasher(const std::string& filename, std::size_t block_size)
        : filename_(filename), block_size_(block_size)
    {
        if (block_size_ == 0) {
            block_size_ = 1;
        }
    }
    Hash getNextBlock();
private:
    std::string filename_;
    std::ifstream in_;
    std::size_t block_size_ = 1;
    bool is_end = false;
    std::size_t blocks_cnt = 0;
    std::vector<Hash> blocks_cache_;
};