#include "file_cmp.h"

#include <utility>
#include <unordered_map>

//# define TEST

#ifdef TEST
#include <iostream>
#endif

using namespace std;

CompareFiles::DuplicateList CompareFiles::compare(const std::vector<std::string> &files) {
    if (files.size() < 2) {
        return {};
    }

    unordered_map<Hash, vector<pair<int, FileHasher>>> first_byte_hash_to_filehash{};
    DuplicateGroup empty_files{};
    DuplicateList duplicates{};

    for (const auto& cur_fname : files) {
#ifdef TEST
        cerr << cur_fname << endl;
#endif
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
        // add cur file_hash to first_byte_hash if unique
        constexpr int NOT_IN_GROUP = -1;
        auto fb_hash = *fb_hash_opt;
        auto& file_hashers =  first_byte_hash_to_filehash[fb_hash];
        bool isFound = false;
        for (auto& [group_idx, fh] : file_hashers) {
            if (cur_fh == fh) {
                if (group_idx != NOT_IN_GROUP) {
                    duplicates[group_idx].insert(cur_fname);
                } else {
                    group_idx = duplicates.size();
                    duplicates.push_back({fh.getFileName(), cur_fname});
                }
                isFound = true;
                break;
            }
        }
        if (!isFound) {
            file_hashers.emplace_back(NOT_IN_GROUP, (move(cur_fh)));
        }
    }
    // add empty file list
    if (!empty_files.empty()) {
        duplicates.push_back(move(empty_files));
    }
    return duplicates;
}

