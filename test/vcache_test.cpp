#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <ostream>

#include "vcache.hpp"

namespace std
{

// helper functions so sets and lists can be printed

ostream & operator<<(ostream & stream, set<int> const & data)
{
    stream << "std::set( ";
    BOOST_FOREACH(int elem, data) {
        stream << elem << " ";
    };
    stream << ")";
    return stream;
};

ostream & operator<<(ostream & stream, list<int> const & data)
{
    stream << "std::list( ";
    BOOST_FOREACH(int elem, data) {
        stream << elem << " ";
    };
    stream << ")";
    return stream;
};

}

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

BOOST_AUTO_TEST_CASE(triangle_info_test)
{
    BOOST_CHECK_THROW(TriangleInfo(std::list<int>()), std::runtime_error);
    BOOST_CHECK_THROW(TriangleInfo(std::list<int>(1)), std::runtime_error);
    BOOST_CHECK_THROW(TriangleInfo(std::list<int>(2)), std::runtime_error);
    BOOST_CHECK_NO_THROW(TriangleInfo(std::list<int>(3)));
    BOOST_CHECK_THROW(TriangleInfo(std::list<int>(4)), std::runtime_error);
    BOOST_CHECK_THROW(TriangleInfo(std::list<int>(5)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(mesh_empty_test)
{
    std::list<std::list<int> > triangles;
    VertexScore vertex_score;
    BOOST_CHECK_NO_THROW(Mesh(triangles, vertex_score));
}

BOOST_AUTO_TEST_CASE(mesh_degenerate_test)
{
    std::list<int> triangle;
    std::list<std::list<int> > triangles;
    VertexScore vertex_score;
    // construct mesh
    triangle.clear();
    triangle.push_back(0);
    triangle.push_back(1);
    triangle.push_back(2);
    triangles.push_back(triangle);
    triangle.clear();
    triangle.push_back(1);
    triangle.push_back(2);
    triangle.push_back(0);
    triangles.push_back(triangle);
    Mesh mesh(triangles, vertex_score);
    // test vertex infos
    std::set<int> indices;
    indices.clear();
    indices.insert(0);
    BOOST_CHECK_EQUAL(mesh.vertex_infos.size(), 3);
    BOOST_CHECK_EQUAL(mesh.vertex_infos[0].triangle_indices, indices);
    BOOST_CHECK_EQUAL(mesh.vertex_infos[1].triangle_indices, indices);
    BOOST_CHECK_EQUAL(mesh.vertex_infos[2].triangle_indices, indices);
    // test triangle infos
    BOOST_CHECK_EQUAL(mesh.triangle_infos.size(), 1);
    triangle.clear();
    triangle.push_back(0);
    triangle.push_back(1);
    triangle.push_back(2);
    BOOST_CHECK_EQUAL(mesh.triangle_infos[0].vertex_indices, triangle);
}

BOOST_AUTO_TEST_CASE(mesh_double_test)
{
    std::list<int> triangle;
    std::list<std::list<int> > triangles;
    VertexScore vertex_score;
    // construct mesh
    triangle.clear();
    triangle.push_back(0);
    triangle.push_back(1);
    triangle.push_back(2);
    triangles.push_back(triangle);
    triangle.clear();
    triangle.push_back(2);
    triangle.push_back(1);
    triangle.push_back(3);
    triangles.push_back(triangle);
    Mesh mesh(triangles, vertex_score);
    // test vertex infos
    std::set<int> indices;
    BOOST_CHECK_EQUAL(mesh.vertex_infos.size(), 4);
    indices.clear();
    indices.insert(0);
    BOOST_CHECK_EQUAL(mesh.vertex_infos[0].triangle_indices, indices);
    indices.clear();
    indices.insert(0);
    indices.insert(1);
    BOOST_CHECK_EQUAL(mesh.vertex_infos[1].triangle_indices, indices);
    BOOST_CHECK_EQUAL(mesh.vertex_infos[2].triangle_indices, indices);
    indices.clear();
    indices.insert(1);
    BOOST_CHECK_EQUAL(mesh.vertex_infos[3].triangle_indices, indices);
    // test triangle infos
    BOOST_CHECK_EQUAL(mesh.triangle_infos.size(), 2);
    triangle.clear();
    triangle.push_back(0);
    triangle.push_back(1);
    triangle.push_back(2);
    BOOST_CHECK_EQUAL(mesh.triangle_infos[0].vertex_indices, triangle);
    triangle.clear();
    triangle.push_back(2);
    triangle.push_back(1);
    triangle.push_back(3);
    BOOST_CHECK_EQUAL(mesh.triangle_infos[1].vertex_indices, triangle);
}

BOOST_AUTO_TEST_SUITE_END()
