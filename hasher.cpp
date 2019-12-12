#include "hasher.h"

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

Hash Md5Hasher::operator()(const std::vector<char> &v) {
    MD5_CTX md5handler;
    unsigned char md5digest[MD5_DIGEST_LENGTH];

    MD5_Init(&md5handler);
    MD5_Update(&md5handler, v.data(), v.size());
    MD5_Final(md5digest, &md5handler);

    return 0; //md5digest;
}
