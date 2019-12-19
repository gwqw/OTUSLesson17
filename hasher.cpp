#include "hasher.h"

#include <boost/crc.hpp>

using namespace std;

Hash Crc32Hasher::operator()(const std::vector<char>& v) {
    boost::crc_32_type result;
    result.process_bytes(v.data(), v.size());
    return result.checksum();
}

HasherHolder makeHasher(HashType hash_type) {
    switch (hash_type) {
        case HashType::Boost:
            return make_unique<BoostHasher>();
        case HashType::CRC32:
            return make_unique<Crc32Hasher>();
        default:
            throw runtime_error("Unknown hash type");
    }
}
