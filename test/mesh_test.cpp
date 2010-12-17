#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <ostream>

#include "vcache.hpp" // TODO replace by vcache/mesh.hpp and update tests
#include "test_helpers.hpp"

BOOST_AUTO_TEST_SUITE(mesh_test_suite)

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
