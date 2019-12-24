#include <iostream>
#include <boost/program_options.hpp>

#include "parse_command_options.h"
#include "file_cmp.h"
#include "hasher.h"


using namespace std;
namespace opt = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        auto optional_options = parse_command_oprions(argc, argv);
        if (!optional_options) {
            return 0;
        }
        auto [files, block_size, hashType] = *optional_options;

        auto hasher = makeHasher(hashType);
        CompareFiles fileComparator(block_size, *hasher);

        cout << fileComparator.compare(files);
    }
    catch (const opt::error& e) {
        cerr << e.what() << endl;
        return 1;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 2;
    }

    return 0;
}
