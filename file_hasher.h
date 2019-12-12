#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <optional>

#include "hasher.h"

using namespace std;

class FileHasher {
    using Block = std::vector<char>;
public:
    explicit FileHasher(std::string filename, std::size_t block_size, IHasher& hasher)
        : filename_(std::move(filename)), block_size_(block_size), hasher_(hasher)
    {
        if (block_size_ == 0) {
            block_size_ = 1;
        }
        //in_.rdbuf()->pubsetbuf(nullptr, 0);
    }
    FileHasher(const FileHasher&) = delete;
    FileHasher(FileHasher&&) = default;
    FileHasher& operator=(const FileHasher&) = delete;
    FileHasher& operator=(FileHasher&&) = delete; // because of IHasher&

    std::optional<Hash> operator[](std::size_t idx);

    const std::string& getFileName() const {return filename_;}

    std::size_t getFileSize();

private:
    std::string filename_;
    //std::ifstream in_;
    std::size_t block_size_ = 1;
    std::vector<Hash> blocks_cache_;
    IHasher& hasher_;
    std::optional<std::size_t> file_size_;

    // methods
    Hash getNext(std::ifstream& in);
    Hash getFullHash() const {
        return boost::hash<std::vector<Hash>>{}(blocks_cache_);
    }
};

bool operator==(FileHasher& lhs, FileHasher& rhs);