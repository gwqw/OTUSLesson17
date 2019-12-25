#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <boost/optional.hpp>

#include "hasher.h"
#include "file_utils.h"

using namespace std;

class FileHasher {
    using Block = std::vector<char>;

    class BlockFile {
    public:
        BlockFile(std::string filename, std::size_t block_size)
            : filename_(std::move(filename)), block_size_(block_size)
        {
            file_size_ = get_file_size(filename_);
        }

        void open();
        void close();

        bool is_valid() const { return static_cast<bool>(in_);}
        void shift(std::size_t blocks_num);
        Block read();
        // getters
        const std::string& getFileName() const {return filename_;}
        std::size_t getFileSize() const { return file_size_;}
    private:
        std::string filename_;
        std::size_t block_size_ = 0;
        std::size_t file_size_ = 0;
        std::ifstream in_;
        std::size_t readed_blocks_ = 0;
    };

public:
    explicit FileHasher(std::string filename, std::size_t block_size, IHasher& hasher)
        : block_size_(block_size), hasher_(hasher),
        bfile_(std::move(filename), block_size)
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
    boost::optional<Hash> readBlock(std::size_t block_num);
    // getters
    [[nodiscard]] const std::string& getFileName() const {return bfile_.getFileName();}
    std::size_t getFileSize() const;
    void openBlockFile() {
        bfile_.open();
    }
    void closeBlockFile() {
        bfile_.close();
    }
private:
    std::size_t block_size_ = 1;
    std::vector<Hash> blocks_cache_;
    IHasher& hasher_;
    BlockFile bfile_;
};

bool operator==(FileHasher& lhs, FileHasher& rhs);
