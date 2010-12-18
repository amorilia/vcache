#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <ostream>

#include "vcache.hpp"
#include "test_helpers.hpp"

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
    valence = VCACHE_VALENCE_SIZE;
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
    // corner cases: negative valence value
    valence = -1;
    cache_position = 0;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
    cache_position = 1;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
    cache_position = 2;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
    cache_position = 3;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
    cache_position = 4;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
    cache_position = 5;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
    // corner cases: cache position exceeds modelled cache
    cache_position = VCACHE_CACHE_SIZE;
    valence = 0;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
    valence = 1;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
    valence = 2;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
    valence = 3;
    BOOST_CHECK_THROW(vertex_score.get(cache_position, valence), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(optimize_test_1)
{
    int raw_faces[][3] = {
        {1, 5, 2},
        {5, 2, 6},
        {5, 9, 6},
        {9, 6, 10},
        {9, 13, 10},
        {13, 10, 14},
        {0, 4, 1},
        {4, 1, 5},
        {4, 8, 5},
        {8, 5, 9},
        {8, 12, 9},
        {12, 9, 13},
        {2, 6, 3},
        {6, 3, 7},
        {6, 10, 7},
        {10, 7, 11},
        {10, 14, 11},
        {14, 11, 15}
    };
    VertexScore vertex_score;
    std::list<std::list<int> > faces;
    BOOST_FOREACH(int *raw_face, raw_faces) {
        std::list<int> face;
        face.push_back(raw_face[0]);
        face.push_back(raw_face[1]);
        face.push_back(raw_face[2]);
        faces.push_back(face);
    };
    BOOST_CHECK_EQUAL(get_average_transform_to_vertex_ratio(faces, 8), 1.5f);
    std::list<std::list<int> > opt_faces = get_cache_optimized_faces(faces);
    BOOST_CHECK_EQUAL(faces.size(), opt_faces.size());
    BOOST_CHECK_EQUAL(get_average_transform_to_vertex_ratio(opt_faces, 8), 1.0f);
}

BOOST_AUTO_TEST_SUITE_END()
