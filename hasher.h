#pragma once

#include <vector>
#include <memory>
#include <boost/crc.hpp>
#include <boost/container_hash/hash.hpp>

using Hash = std::size_t;
enum class HashType {Boost, CRC32, MD5};

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

class TestHasher : public IHasher {
public:
    Hash operator()(const std::vector<char>& a) override {
        Hash res = 0;
        for (auto c : a) {
            res  = (res + c) % 256;
        }
        return res;
    }
};

HasherHolder makeHasher(HashType hash_type);

