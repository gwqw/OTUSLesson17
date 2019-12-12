#include "file_utils.h"

#include <algorithm>
#include <boost/filesystem.hpp>
#include <iostream>

using namespace std;
namespace fs = boost::filesystem;

void make_full_paths(std::vector<std::string>& files) {
    fs::path cur_path = fs::current_path();
    transform(files.begin(), files.end(), files.begin(), [cur_path](string fname){
        return cur_path.string() + "/" + fname;
    });
}

void remove_non_valid_paths(std::vector<std::string>& files) {
    files.erase(
            remove_if(files.begin(), files.end(), [](const string& f){
                return !fs::exists(f);}),
            files.end()
    );
}

void sort_names_and_remove_duplic(std::vector<std::string>& files) {
    sort(files.begin(), files.end());
    files.erase(unique(files.begin(), files.end()), files.end());
}

std::vector<std::string> getFileListRecursive(const std::string& path_name) {
    if (!fs::exists(path_name)) return {};
    fs::path parent_path(path_name);
    vector<string> res;
    try {
        for (const auto& p : fs::recursive_directory_iterator(parent_path)) {
            if (fs::is_regular_file(p.path())) {
                res.push_back(p.path().string());
            }
        }
    } catch (const exception& e) {
        cerr << e.what() << " " << res.back() << endl;
        return res;
    }
    return res;
}
