#define BOOST_TEST_MODULE bayan_test_module
#include <boost/test/unit_test.hpp>

#include <array>
#include <vector>
#include <iostream>
#include <cstdint>

#include "hasher.h"
#include "file_cmp.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(bayan_test_suite)

    BOOST_AUTO_TEST_CASE(test_hash) {
        const vector<char> v1{'1','1','2'};
        const vector<char> v2{'1','2','1'};
        {
            auto hash_ptr = makeHasher(HashType::Boost);
            auto& hasher = *hash_ptr;
            auto h1 = hasher(v1);
            auto h2 = hasher(v2);
            BOOST_CHECK(h1 != h2);
        }
        {
            auto hash_ptr = makeHasher(HashType::Boost);
            auto& hasher = *hash_ptr;
            auto h1 = hasher(v1);
            auto h2 = hasher(v1);
            BOOST_CHECK(h1 == h2);
        }
        {
            auto hash_ptr = makeHasher(HashType::CRC32);
            auto& hasher = *hash_ptr;
            auto h1 = hasher(v1);
            auto h2 = hasher(v2);
            BOOST_CHECK(h1 != h2);
        }
        {
            auto hash_ptr = makeHasher(HashType::CRC32);
            auto& hasher = *hash_ptr;
            auto h1 = hasher(v1);
            auto h2 = hasher(v1);
            BOOST_CHECK(h1 == h2);
        }
    }

    BOOST_AUTO_TEST_CASE(test_bayan) {
        auto hasher = makeHasher(HashType::Boost);
        {
            vector<string> files{"tests/test1_1.txt", "tests/test1_2.txt"};
            CompareFiles fileComparator(2, *hasher);
            auto res = fileComparator.compare(files);
            BOOST_CHECK(res.empty());
        }
        {
            vector<string> files{"tests/test2_1.txt", "tests/test2_2.txt"};
            CompareFiles fileComparator(2, *hasher);
            auto res = fileComparator.compare(files);
            BOOST_CHECK(res.empty());
        }
        {
            vector<string> files{"tests/test3_1.txt", "tests/test3_2.txt"};
            CompareFiles fileComparator(2, *hasher);
            auto res = fileComparator.compare(files);
            BOOST_CHECK(res.empty());
        }
        {// empty files
            vector<string> files{"tests/test4_1.txt", "tests/test4_2.txt"};
            CompareFiles fileComparator(2, *hasher);
            auto res = fileComparator.compare(files);
            BOOST_CHECK(res.size() == 1u);
            BOOST_CHECK(res.at(0).size() == 2u);
        }
        {// zero trailing
            vector<string> files{"tests/test5_1.txt", "tests/test5_2.txt"};
            CompareFiles fileComparator(2, *hasher);
            auto res = fileComparator.compare(files);
            BOOST_CHECK(res.empty());
        }
        {// equal files
            vector<string> files{"tests/test1_1.txt", "tests/test2_1.txt",
                                 "tests/test3_1.txt", "tests/test5_1.txt"};
            CompareFiles fileComparator(2, *hasher);
            auto res = fileComparator.compare(files);
            BOOST_CHECK(res.size() == 1u);
            BOOST_CHECK(res.at(0).size() == 3u);
        }
        {// different sizes
            vector<string> files{"tests/test7_1.txt", "tests/test7_2.txt"};
            CompareFiles fileComparator(2, *hasher);
            auto res = fileComparator.compare(files);
            BOOST_CHECK(res.empty());
        }
        {// 2 equal groups
            vector<string> files{"tests/test1_1.txt", "tests/test2_1.txt",
                                 "tests/test6_1.txt", "tests/test6_2.txt"};
            CompareFiles fileComparator(3, *hasher);
            auto res = fileComparator.compare(files);
            BOOST_CHECK(res.size() == 2u);
            BOOST_CHECK(res.at(0).size() == 2u);
        }
    }


BOOST_AUTO_TEST_SUITE_END()
