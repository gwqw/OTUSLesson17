#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <boost/optional.hpp>

#include "hasher.h"

using namespace std;

class FileHasher {
    using Block = std::vector<char>;

    class BlockFile {
    public:
        BlockFile(const std::string& filename, std::size_t block_size, std::size_t file_size)
                : filename_(filename), block_size_(block_size), file_size_(file_size) {
            in_.rdbuf()->pubsetbuf(nullptr, 0);
            in_.open(filename_, std::ios::binary|std::ios_base::in);
        }

        void shift(std::size_t blocks_num);

        bool is_valid() const {
            return static_cast<bool>(in_);
        }

        Block read();
    private:
        const std::string& filename_;
        std::size_t block_size_ = 0;
        std::size_t file_size_ = 0;
        std::ifstream in_;
        std::size_t readed_blocks = 0;
    };

public:
    explicit FileHasher(std::string filename, std::size_t block_size, IHasher& hasher)
        : filename_(std::move(filename)), block_size_(block_size), hasher_(hasher)
    {
        if (block_size_ == 0) {
            throw invalid_argument("Block size can't be zero");
        }
    }
    FileHasher(const FileHasher&) = delete;
    FileHasher(FileHasher&&) = default;
    FileHasher& operator=(const FileHasher&) = delete;
    FileHasher& operator=(FileHasher&&) = delete; // because of IHasher&
    // access
    boost::optional<Hash> operator[](std::size_t idx);
    boost::optional<Hash> readBlock(std::size_t block_num, BlockFile& blockFile);
    // getters
    [[nodiscard]] const std::string& getFileName() const {return filename_;}
    std::size_t getFileSize();
    BlockFile getBlockFile() {
        return BlockFile(filename_, block_size_, getFileSize());
    }

private:
    std::string filename_;
    std::size_t block_size_ = 1;
    std::vector<Hash> blocks_cache_;
    IHasher& hasher_;
    boost::optional<std::size_t> file_size_;
};

bool operator==(FileHasher& lhs, FileHasher& rhs);