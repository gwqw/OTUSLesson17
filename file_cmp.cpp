#include "file_cmp.h"

#include <boost/move/utility.hpp>
#include <boost/unordered_map.hpp>

//# define TEST

#ifdef TEST
#include <iostream>
#endif

//using namespace std;

CompareFiles::DuplicateList CompareFiles::compare(const std::vector<std::string> &files) {
    if (files.size() < 2) {
        return {};
    }

    boost::unordered_map<size_t,
        boost::container::vector<pair<int, FileHasher>>> size_to_filehash{};
    DuplicateGroup empty_files{};
    DuplicateList duplicates{};

    for (const auto& cur_fname : files) {
#ifdef TEST
        cerr << cur_fname << endl;
#endif
        FileHasher cur_fh(cur_fname, block_size_, hasher_);
        // file is empty
        auto cur_size = cur_fh.getFileSize();
        if (cur_size == 0) {
            empty_files.insert(cur_fname);
            continue;
        }
        // Algo:
        // select filehashes with the same size
        // and compare with them
        // if true: add to duplicates
        // add cur file_hash to size_to_filehash if unique
        constexpr int NOT_IN_GROUP = -1;
        auto& file_hashers =  size_to_filehash[cur_size];
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
        duplicates.push_back(boost::move(empty_files));
    }
    return duplicates;
}


std::ostream& operator<<(std::ostream& out, const CompareFiles::DuplicateList& dupList) {
    bool is_first = true;
    for (const auto& file_set : dupList) {
        if (is_first) {
            is_first = false;
        } else {
            out << '\n';
        }
        for (const auto& f : file_set) {
            out << f << '\n';
        }
    }
    return out;
}