#include "../source/utilities/sized_array.hpp"
#include "../source/server/memory_pool.hpp"

#include <iostream>
#include <boost/test/unit_test.hpp>

namespace nova
{
simple_pool<false> rt_pool;
} /* namespace nova */

using namespace nova;

BOOST_AUTO_TEST_CASE( sized_array_test_1 )
{
    sized_array<int> array(5);
    array[0] = -1;
    array[1] = 3;
    array[4] = 44;

    BOOST_REQUIRE_EQUAL( array.size(), 5u );
    BOOST_REQUIRE_EQUAL( array[1], 3 );
    BOOST_REQUIRE_EQUAL( array[0], -1 );
    BOOST_REQUIRE_EQUAL( *array.begin(), -1 );
    BOOST_REQUIRE_EQUAL( array.front(), -1 );
    BOOST_REQUIRE_EQUAL( array.back(), 44 );
    BOOST_REQUIRE_EQUAL( *array.rbegin(), 44 );
    BOOST_REQUIRE_EQUAL( *(array.rend()-1), -1 );

    sized_array<long> long_array(array);
    BOOST_REQUIRE_EQUAL( long_array.size(), 5u );
    BOOST_REQUIRE_EQUAL( long_array[1], 3 );
    BOOST_REQUIRE_EQUAL( long_array[0], -1 );

    // move assignment
    sized_array<int> array3(0);
    boost::move(&array, &array+1, &array3);
    BOOST_REQUIRE_EQUAL( array3.size(), 5u );
    BOOST_REQUIRE_EQUAL( array3[1], 3 );
    BOOST_REQUIRE_EQUAL( array3[0], -1 );
    BOOST_REQUIRE_EQUAL( array.size(), 0u );

    // move assignment
    sized_array<int> array4(boost::move(array3));
    BOOST_REQUIRE_EQUAL( array4.size(), 5u );
    BOOST_REQUIRE_EQUAL( array4[1], 3 );
    BOOST_REQUIRE_EQUAL( array4[0], -1 );
    BOOST_REQUIRE_EQUAL( array3.size(), 0u );
}


template <typename Alloc1, typename Alloc2>
void run_test_2(void)
{
    int size = 1024;

    std::vector<int, Alloc1> vec;
    for (int i = 0; i != size; ++i)
        vec.push_back(-i);

    sized_array<int, Alloc2> array(vec);

    for (int i = 0; i != size; ++i)
        BOOST_REQUIRE_EQUAL( vec[i], array[i] );
}

BOOST_AUTO_TEST_CASE( sized_array_test_2 )
{
    rt_pool.init(1024*1024);
    run_test_2<std::allocator<void*>, std::allocator<void*> >();
    run_test_2<rt_pool_allocator<void*>, std::allocator<void*> >();
    run_test_2<std::allocator<void*>, rt_pool_allocator<void*> >();
    run_test_2<rt_pool_allocator<void*>, rt_pool_allocator<void*> >();
}
