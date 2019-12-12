#pragma once

#include <vector>
#include <string>
#include <unordered_set>

#include "file_hasher.h"
#include "hasher.h"

class CompareFiles {
public:
    using DuplicateGroup = std::unordered_set<std::string>;
    using DuplicateList = std::vector<DuplicateGroup>;
    explicit CompareFiles(std::size_t block_size, IHasher& hasher)
        : block_size_(block_size), hasher_(hasher) {}
    DuplicateList compare(const std::vector<std::string>& files);
private:
    std::size_t block_size_ = 1;
    IHasher& hasher_;
};
