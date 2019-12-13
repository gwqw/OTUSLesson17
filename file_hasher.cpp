#include "file_hasher.h"

using namespace std;

// Block File
void FileHasher::BlockFile::shift(std::size_t blocks_num) {
    if (blocks_num > readed_blocks) {
        std::size_t new_pos = (blocks_num - readed_blocks) * block_size_;
        in_.seekg(new_pos);
        readed_blocks = blocks_num;
    }
}

FileHasher::Block FileHasher::BlockFile::read() {
    Block b(block_size_);
    std::fill(b.begin(), b.end(), 0);
    if ((readed_blocks + 1) * block_size_ < file_size_) {
        in_.read(b.data(), block_size_);
    } else {
        std::size_t readed = 0;
        while (in_ && readed < block_size_) {
            in_.read(b.data() + readed, 1);
            ++readed;
        }
        ++readed_blocks;
    }
    return b;
}

// FileHasher
std::optional<Hash> FileHasher::readBlock(std::size_t block_num,
        FileHasher::BlockFile &blockFile) {
    if (block_num < blocks_cache_.size()) {
        return blocks_cache_[block_num];
    }
    if (block_num * block_size_ >= getFileSize() || !blockFile.is_valid()) {
        return nullopt;
    }

    blockFile.shift(blocks_cache_.size());
    //blocks_cache_.reserve(block_num + 1);
    while (blocks_cache_.size() <= block_num && blockFile.is_valid()) {
        blocks_cache_.push_back(hasher_(blockFile.read()));
    }

    if (block_num < blocks_cache_.size()) {
        return blocks_cache_[block_num];
    } else {
        return nullopt;
    }
}

std::optional<Hash> FileHasher::operator[](std::size_t idx) {
    auto block_file = getBlockFile();
    return readBlock(idx, block_file);
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
    auto lsh_bfh = lhs.getBlockFile();
    auto rsh_bfh = rhs.getBlockFile();
    for (size_t i = 0;
        lhs.readBlock(i, lsh_bfh).has_value() && rhs.readBlock(i, rsh_bfh).has_value();
        ++i) {
        if (*lhs[i] != *rhs[i]) {
            res = false;
            break;
        }
    }
    return res;
}

