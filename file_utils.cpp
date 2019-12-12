#include "file_utils.h"

#include <algorithm>
#include <boost/filesystem.hpp>

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