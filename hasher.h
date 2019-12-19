#pragma once

#include <vector>
#include <memory>

#include <boost/version.hpp>
#if  BOOST_VERSION >= 106700
#include <boost/container_hash/hash.hpp>
#else
#include <boost/functional/hash.hpp>
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
    Hash operator()(const std::vector<char>& v) override {return hasher_(v);}
private:
    boost::hash<std::vector<char>> hasher_;
};

class Crc32Hasher : public IHasher {
public:
    Hash operator()(const std::vector<char>& v) override;
};

HasherHolder makeHasher(HashType hash_type);


