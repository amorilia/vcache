#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <iostream>
#include <fstream>

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
    int raw_faces[18][3] = {
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
    int raw_opt_faces[18][3] = {
        {0, 4, 1},
        {1, 5, 4},
        {1, 5, 2},
        {4, 8, 5},
        {2, 6, 5},
        {2, 6, 3},
        {3, 7, 6},
        {5, 9, 8},
        {5, 9, 6},
        {8, 12, 9},
        {9, 13, 12},
        {6, 10, 7},
        {6, 10, 9},
        {9, 13, 10},
        {7, 11, 10},
        {10, 14, 13},
        {10, 14, 11},
        {11, 15, 14}
    };
    VertexScore vertex_score;
    std::list<std::list<int> > faces = array_to_list<int, 18, 3>(raw_faces);
    std::list<std::list<int> > opt_faces = array_to_list<int, 18, 3>(raw_opt_faces);
    std::pair<int, int> tvr;
    tvr = get_transform_to_vertex_ratio(faces, 8);
    BOOST_CHECK_EQUAL(tvr.first, 24);
    BOOST_CHECK_EQUAL(tvr.second, 16);
    tvr = get_transform_to_vertex_ratio(opt_faces, 8);
    BOOST_CHECK_EQUAL(tvr.first, 16);
    BOOST_CHECK_EQUAL(tvr.second, 16);
    BOOST_CHECK_EQUAL(opt_faces, get_cache_optimized_faces(faces));
}

BOOST_AUTO_TEST_CASE(optimize_test_suzanne)
{
    VertexScore vertex_score;
    std::ifstream stream(TEST_PATH "/obj/suzanne.obj");
    std::list<std::list<int> > faces = obj_faces(stream);
    //std::cout << faces << std::endl;
    std::list<std::list<int> > opt_faces = get_cache_optimized_faces(faces);
    std::pair<int, int> tvr;
    tvr = get_transform_to_vertex_ratio(faces, 32);
    BOOST_CHECK_EQUAL(tvr.first, 3092);
    BOOST_CHECK_EQUAL(tvr.second, 2012);
    tvr = get_transform_to_vertex_ratio(opt_faces, 32);
    BOOST_CHECK_EQUAL(tvr.first, 2637);
    BOOST_CHECK_EQUAL(tvr.second, 2012);
}

BOOST_AUTO_TEST_CASE(optimize_test_grid_48x48)
{
    VertexScore vertex_score;
    std::ifstream stream(TEST_PATH "/obj/grid-48x48.obj");
    std::list<std::list<int> > faces = obj_faces(stream);
    //std::cout << faces << std::endl;
    std::list<std::list<int> > opt_faces = get_cache_optimized_faces(faces);
    std::pair<int, int> tvr;
    tvr = get_transform_to_vertex_ratio(faces, 32);
    BOOST_CHECK_EQUAL(tvr.first, 4052);
    BOOST_CHECK_EQUAL(tvr.second, 2304);
    tvr = get_transform_to_vertex_ratio(opt_faces, 32);
    BOOST_CHECK_EQUAL(tvr.first, 3021);
    BOOST_CHECK_EQUAL(tvr.second, 2304);
    tvr = get_transform_to_vertex_ratio(faces, 16);
    BOOST_CHECK_EQUAL(tvr.first, 4305);
    BOOST_CHECK_EQUAL(tvr.second, 2304);
    tvr = get_transform_to_vertex_ratio(opt_faces, 16);
    BOOST_CHECK_EQUAL(tvr.first, 3058);
    BOOST_CHECK_EQUAL(tvr.second, 2304);
    tvr = get_transform_to_vertex_ratio(faces, 8);
    BOOST_CHECK_EQUAL(tvr.first, 4420);
    BOOST_CHECK_EQUAL(tvr.second, 2304);
    tvr = get_transform_to_vertex_ratio(opt_faces, 8);
    BOOST_CHECK_EQUAL(tvr.first, 3362);
    BOOST_CHECK_EQUAL(tvr.second, 2304);
}

BOOST_AUTO_TEST_SUITE_END()
