#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <ostream>

#include "vcache/mesh.hpp"
#include "test_helpers.hpp"

BOOST_AUTO_TEST_SUITE(mesh_test_suite)

BOOST_AUTO_TEST_CASE(declare_test)
{
    // check that declarations work as expected
    BOOST_CHECK_NO_THROW(Face face(0, 1, 2));
    BOOST_CHECK_NO_THROW(Mesh mesh);

    // degenerate faces
    BOOST_CHECK_THROW(Face face(30, 0, 30), std::runtime_error);
    BOOST_CHECK_THROW(Face face(0, 40, 40), std::runtime_error);
    BOOST_CHECK_THROW(Face face(50, 50, 0), std::runtime_error);
    BOOST_CHECK_THROW(Face face(7, 7, 7), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(face_vertex_order_test_0)
{
    Face f(6, 2, 5);
    BOOST_CHECK_EQUAL(f.v0, 2);
    BOOST_CHECK_EQUAL(f.v1, 5);
    BOOST_CHECK_EQUAL(f.v2, 6);
}

BOOST_AUTO_TEST_CASE(face_vertex_order_test_1)
{
    Face f(2, 5, 6);
    BOOST_CHECK_EQUAL(f.v0, 2);
    BOOST_CHECK_EQUAL(f.v1, 5);
    BOOST_CHECK_EQUAL(f.v2, 6);
}

BOOST_AUTO_TEST_CASE(face_vertex_order_test_2)
{
    Face f(5, 6, 2);
    BOOST_CHECK_EQUAL(f.v0, 2);
    BOOST_CHECK_EQUAL(f.v1, 5);
    BOOST_CHECK_EQUAL(f.v2, 6);
}

BOOST_AUTO_TEST_CASE(mesh_add_face_test)
{
    Mesh m;

    // add faces
    BOOST_CHECK_NO_THROW(m.add_face(0, 1, 2));
    BOOST_CHECK_NO_THROW(m.add_face(2, 1, 3));
    BOOST_CHECK_NO_THROW(m.add_face(2, 3, 4));
    BOOST_CHECK_EQUAL(m.mfaces.size(), 3);
    BOOST_CHECK_EQUAL(m._vertices.size(), 5);
    // add duplicate face
    BOOST_CHECK_NO_THROW(m.add_face(2, 3, 4));
    MFacePtr f0 = m.add_face(10, 11, 12);
    MFacePtr f1 = m.add_face(12, 10, 11);
    MFacePtr f2 = m.add_face(11, 12, 10);
    BOOST_CHECK_EQUAL(f0, f1);
    BOOST_CHECK_EQUAL(f0, f2);
    // one extra face, four extra vertices
    BOOST_CHECK_EQUAL(m.mfaces.size(), 4);
    BOOST_CHECK_EQUAL(m._vertices.size(), 8);
}

/*
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
*/

BOOST_AUTO_TEST_SUITE_END()
