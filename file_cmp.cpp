#include "file_cmp.h"

#include <utility>
#include <unordered_map>

using namespace std;

CompareFiles::DuplicateList CompareFiles::compare(const std::vector<std::string> &files) {
    if (files.size() < 2) {
        return {};
    }

    unordered_map<Hash, vector<FileHasher>> first_byte_hash_to_filehash{};
    std::unordered_set<std::string> empty_files{};
    DuplicateList duplicates{};

    for (const auto& cur_fname : files) {
        FileHasher cur_fh(cur_fname, block_size_, hasher_);
        // file is empty
        auto fb_hash_opt = cur_fh[0];
        if (!fb_hash_opt) {
            empty_files.insert(cur_fname);
            continue;
        }
        // Algo:
        // select filehashes with the same first byte hash
        // and compare with them
        // if true: add to duplicates
        // add cur file_hash to first_byte_hash
        auto fb_hash = *fb_hash_opt;
        auto& file_hashers =  first_byte_hash_to_filehash[fb_hash];
        for (auto& fh : file_hashers) {
            if (cur_fh == fh) {
                insertDuplicates(duplicates, fh.getFileName(), cur_fname);
            }
        }
        file_hashers.push_back(move(cur_fh));
    }

    // add empty file list
    if (!empty_files.empty()) {
        duplicates.push_back(move(empty_files));
    }
    return duplicates;
}

void CompareFiles::insertDuplicates(CompareFiles::DuplicateList& duplicates,
        const std::string& fname1, const std::string& fname2) {
    bool is_found = false;
    for (auto& s : duplicates) {
        if (s.count(fname1)) {
            s.insert(fname2);
            is_found = true;
            break;
        }
    }
    if (!is_found) {
        duplicates.push_back({fname1, fname2});
    }
}

