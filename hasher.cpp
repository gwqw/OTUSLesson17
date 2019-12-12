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
        case HashType::MD5:
            throw runtime_error("This hash type is unrealized yet");
        default:
            throw runtime_error("Unknown hash type");
    }
}

Md5Hasher::hash_t Md5Hasher::operator()(const std::vector<char> &v) {
    using boost::uuids::detail::md5;

    md5 hash;
    md5::digest_type md5digest;

    hash.process_bytes(v.data(), v.size());
    hash.get_digest(md5digest);

    hash_t res;
    for (size_t i = 0; i < sizeof(md5::digest_type); ++i) {
        res[i] = md5digest[i];
    }

    return res;
}
