#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "vcache.hpp"

BOOST_AUTO_TEST_SUITE(vcache_test_suite)

BOOST_AUTO_TEST_CASE(vertex_score_test)
{
    int cache_position;
    int valence;
    VertexScore vertex_score(1.5, 0.75, 2.0, 0.5);
    // usual cases
    cache_position = -1;
    valence = 0;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), -1000);
    valence = 1;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2000);
    valence = 2;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 1414);
    valence = 3;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 1155);
    cache_position = 0;
    valence = 0;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), -1000);
    valence = 1;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2750);
    valence = 2;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2164);
    valence = 3;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 1905);
    cache_position = 1;
    valence = 0;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), -1000);
    valence = 1;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2750);
    valence = 2;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2164);
    valence = 3;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 1905);
    cache_position = 2;
    valence = 0;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), -1000);
    valence = 1;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2750);
    valence = 2;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2164);
    valence = 3;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 1905);
    cache_position = 3;
    valence = 0;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), -1000);
    valence = 1;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 3000);
    valence = 2;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2414);
    valence = 3;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2155);
    cache_position = 4;
    valence = 0;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), -1000);
    valence = 1;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2949);
    valence = 2;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2363);
    valence = 3;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2104);
    cache_position = 5;
    valence = 0;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), -1000);
    valence = 1;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2898);
    valence = 2;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2312);
    valence = 3;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 2053);
    // corner cases: insane valence value
    valence = VCACHE_MAX_VALENCE + 10;
    cache_position = 0;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 750);
    cache_position = 1;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 750);
    cache_position = 2;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 750);
    cache_position = 3;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 1000);
    cache_position = 4;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 949);
    cache_position = 5;
    BOOST_CHECK_EQUAL(vertex_score.get(cache_position, valence), 898);
}

BOOST_AUTO_TEST_SUITE_END()
