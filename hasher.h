#pragma once

#include <vector>
#include <memory>
#include <boost/version.hpp>

#if  BOOST_VERSION >= 106700
#include <boost/container_hash/hash.hpp>
#else
#include <boost/function/hash.hpp>
#endif

//#define MD5
#ifdef MD5
#include <boost/uuid/detail/md5.hpp>
#endif

using Hash = std::size_t;
enum class HashType {Boost, CRC32};

class IHasher {
public:
    virtual ~IHasher() = default;
    virtual Hash operator()(const std::vector<char>&) = 0;
};
using HasherHolder = std::unique_ptr<IHasher>;

class BoostHasher : public  IHasher {
public:
    using hash_t = std::size_t;
    Hash operator()(const std::vector<char>& v) override {return hasher_(v);}
private:
    boost::hash<std::vector<char>> hasher_;
};

class Crc32Hasher : public IHasher {
public:
    using hash_t = std::size_t;
    Hash operator()(const std::vector<char>& v) override;
};

#ifdef MD5
class Md5Hasher {
public:
    using hash_t = std::array<unsigned int, sizeof(boost::uuids::detail::md5::md5::digest_type)>;
    hash_t operator()(const std::vector<char>& v);
};
#endif

HasherHolder makeHasher(HashType hash_type);


