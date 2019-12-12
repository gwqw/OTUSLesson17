#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "file_cmp.h"
#include "file_utils.h"
#include "hasher.h"

//#define TEST

using namespace std;
namespace opt = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        // parse command line
        opt::options_description desc("Usage: bayan [OPTIONS]... [FILE]...\nAll options:");
        desc.add_options()
                ("help,h", "This screen")
                ("blocksize,b", opt::value<int>()->default_value(1), "Block size")
                ("hash,H", opt::value<std::string>(), "Hash type: boost, crc32, md5")
                ("dir,d", opt::value<std::string>(), "Directory, where to search duplicates (recursive search)")
                ("files,f", opt::value<std::vector<std::string>>()->multitoken()->
                        zero_tokens()->composing(), "files to scan, can be used without -f ");
        opt::positional_options_description pos_desc;
        pos_desc.add("files", -1);

        opt::command_line_parser parser{argc, argv};
        parser.options(desc).positional(pos_desc).allow_unregistered();
        opt::parsed_options parsed_options = parser.run();

        opt::variables_map vm;
        opt::store(parsed_options, vm);
        opt::notify(vm);

        vector<string> files;
        int block_size = 1;
        HashType hashType = HashType::Boost;
        if (vm.count("help")) {
            cout << desc << endl;
            return 0;
        }
        if (vm.count("blocksize")) {
            block_size = vm["blocksize"].as<int>();
        }
        if (vm.count("dir")) {
            files = getFileListRecursive(vm["dir"].as<std::string>());
#ifdef TEST
            cout << "Found " << files.size() << " files" << endl;
#endif
        } else if (vm.count("files")) {
            files = vm["files"].as<std::vector<std::string>>();
            make_full_paths(files);
            remove_non_valid_paths(files);
            sort_names_and_remove_duplic(files);
        }
        if (vm.count("hash")) {
            string h = vm["hash"].as<std::string>();
            if (h == "crc32") {
                hashType = HashType::CRC32;
            } else if (h == "md5") {
                hashType = HashType::MD5;
            } else if (h == "boost") {
                //hashType is already HashType::Boost;
            } else {
                cerr << "unknown hash type: " << h
                    << ". Default hash is set. For more info use --help" << endl;
            }
        }


        auto hasher = makeHasher(hashType);

        // output
#ifdef TEST
        cout << "Input file list: \n";
        for (const auto& f : files) {
            cout << f << endl;
        }
        cout << endl;

        cout << "Identical files:\n";
#endif
        CompareFiles fileComparator(block_size, *hasher);
        auto res = fileComparator.compare(files);
        bool is_first = true;
        for (const auto& file_set : res) {
            if (is_first) {
                is_first = false;
            } else {
                cout << '\n';
            }
            for (const auto& f : file_set) {
                cout << f << '\n';
            }
        }
    }
    catch (const opt::error& e) {
        cerr << e.what() << endl;
        return 1;
    }
    catch (const exception & e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
