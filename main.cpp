#include <iostream>
#include <string>
#include <boost/program_options.hpp>

using namespace std;
namespace opt = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        opt::options_description desc("All options");
        desc.add_options()
                ("help,h", "This screen")
                ("files,f", opt::value<std::vector<std::string>>()->multitoken()->
                        zero_tokens()->composing(), "Included dirs");
        opt::positional_options_description pos_desc;
        pos_desc.add("files", -1);

        opt::command_line_parser parser{argc, argv};
        parser.options(desc).positional(pos_desc).allow_unregistered();
        opt::parsed_options parsed_options = parser.run();

        opt::variables_map vm;
        opt::store(parsed_options, vm);
        opt::notify(vm);

        vector<string> files;
        if (vm.count("help")) {
            cout << desc << endl;
            return 0;
        }
        if (vm.count("files")) {
            files = vm["files"].as<std::vector<std::string>>();
        }

        for (const auto& f : files) {
            cout << f << endl;
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
