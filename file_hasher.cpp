#include "file_hasher.h"

#include "file_utils.h"

using namespace std;

// Block File
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
        size_t to_read_size = (readed_blocks_ + 1) * block_size_ - file_size_;
        in_.read(b.data(), to_read_size);
    }
    ++readed_blocks_;
    return b;
}

// FileHasher
boost::optional<Hash> FileHasher::readBlock(std::size_t block_num,
        FileHasher::BlockFile &blockFile) {
    if (block_num < blocks_cache_.size()) {
        return blocks_cache_[block_num];
    }
    if (block_num * block_size_ >= getFileSize() || !blockFile.is_valid()) {
        return boost::none;
    }

    blockFile.shift(blocks_cache_.size());
    //blocks_cache_.reserve(block_num + 1);
    while (blocks_cache_.size() <= block_num && blockFile.is_valid()) {
        blocks_cache_.push_back(hasher_(blockFile.read()));
    }

    if (block_num < blocks_cache_.size()) {
        return blocks_cache_[block_num];
    } else {
        return boost::none;
    }
}

boost::optional<Hash> FileHasher::operator[](std::size_t idx) {
    auto block_file = getBlockFile();
    return readBlock(idx, block_file);
}

std::size_t FileHasher::getFileSize() {
    if (file_size_) {
        return *file_size_;
    }
    file_size_ = get_file_size(filename_);
    return *file_size_;
}

bool operator==(FileHasher &lhs, FileHasher &rhs) {
    if (lhs.getFileSize() != rhs.getFileSize()) {
        return false;
    }
    bool res = true;
    auto lsh_bfh = lhs.getBlockFile();
    auto rsh_bfh = rhs.getBlockFile();
    for (size_t i = 0;
        lhs.readBlock(i, lsh_bfh) && rhs.readBlock(i, rsh_bfh);
        ++i) {
        if (*lhs[i] != *rhs[i]) {
            res = false;
            break;
        }
    }
    return res;
}

