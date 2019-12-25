#include "file_hasher.h"

using namespace std;

// Block File

void FileHasher::BlockFile::open() {
    if (!in_.is_open()) {
        in_.rdbuf()->pubsetbuf(nullptr, 0);
        in_.open(filename_, std::ios::binary | std::ios_base::in);
    }
}

void FileHasher::BlockFile::close() {
    if (in_.is_open()) {
        in_.close();
        readed_blocks_ = 0;
    }
}

void FileHasher::BlockFile::shift(std::size_t blocks_num) {
    if (blocks_num > readed_blocks_) {
        std::size_t new_pos = (blocks_num - readed_blocks_) * block_size_;
        in_.seekg(new_pos);
        readed_blocks_ = blocks_num;
    }
}

FileHasher::Block FileHasher::BlockFile::read() {
    Block b(block_size_);
    std::fill(b.begin(), b.end(), 0);
    if ((readed_blocks_ + 1) * block_size_ <= file_size_) {
        in_.read(b.data(), block_size_);
    } else {
        size_t to_read_size = file_size_ > readed_blocks_ * block_size_ ?
                file_size_ - readed_blocks_ * block_size_
                : 0;
        in_.read(b.data(), to_read_size);
    }
    ++readed_blocks_;
    return b;
}



// FileHasher
boost::optional<Hash> FileHasher::readBlock(std::size_t block_num) {
    if (block_num < blocks_cache_.size()) {
        return blocks_cache_[block_num];
    }
    if (block_num * block_size_ >= getFileSize() || !bfile_.is_valid()) {
        return boost::none;
    }

    openBlockFile();
    bfile_.shift(blocks_cache_.size());
    //blocks_cache_.reserve(block_num + 1);
    while (blocks_cache_.size() <= block_num && bfile_.is_valid()) {
        blocks_cache_.push_back(hasher_(bfile_.read()));
    }

    if (block_num < blocks_cache_.size()) {
        return blocks_cache_[block_num];
    } else {
        return boost::none;
    }
}

boost::optional<Hash> FileHasher::operator[](std::size_t idx) {
    return readBlock(idx);
}

std::size_t FileHasher::getFileSize() const {
    return bfile_.getFileSize();
}

bool operator==(FileHasher &lhs, FileHasher &rhs) {
    if (lhs.getFileSize() != rhs.getFileSize()) {
        return false;
    }

    bool res = true;
    size_t i = 0;
    for (auto b1 = lhs.readBlock(i), b2 = rhs.readBlock(i);
         b1 && b2;
         ++i,
         b1 = lhs.readBlock(i),
         b2 = rhs.readBlock(i)
    ) {
        if (*b1 != *b2) {
            res = false;
            break;
        }
    }

    lhs.closeBlockFile();
    rhs.closeBlockFile();
    return res;
}

