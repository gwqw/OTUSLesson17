#define BOOST_TEST_MODULE bayan_test_module
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <array>
#include <vector>
#include <iostream>
#include <cstdint>

#include "hasher.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(bayan_test_suite)

//	BOOST_AUTO_TEST_CASE(test_bin_reading) {
//        stringstream ss("0123345", ios_base::binary|ios_base::in);
//        constexpr int BLK_SZE = 2;
//        array<char, BLK_SZE> block;
//        fill(block.begin(), block.end(), 0);
//        while(ss.read(block.data(), BLK_SZE)) {
//            cerr << hex << int16_t(block[0]) << " " << int16_t(block[1]) << endl;
//        }
//        while (ss) {
//            char symb;
//            ss.read(&symb, 1);
//            cerr << hex << int16_t(symb) << " ";
//        }
//        cerr << endl;
//	}

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

BOOST_AUTO_TEST_SUITE_END()
