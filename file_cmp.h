#pragma once

#include <string>
#include <vector>
#include <boost/unordered_set.hpp>
#include <boost/container/vector.hpp>
#include <iostream>

#include "file_hasher.h"
#include "hasher.h"

class CompareFiles {
public:
    using DuplicateGroup = boost::unordered_set<std::string>;
    using DuplicateList = boost::container::vector<DuplicateGroup>;
    explicit CompareFiles(std::size_t block_size, IHasher& hasher)
        : block_size_(block_size), hasher_(hasher) {}
    DuplicateList compare(const std::vector<std::string>& files);
private:
    std::size_t block_size_ = 1;
    IHasher& hasher_;
};

std::ostream& operator<<(std::ostream& out, const CompareFiles::DuplicateList& dupList);
