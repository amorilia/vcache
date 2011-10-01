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
    BOOST_CHECK_NO_THROW(Mesh mesh(3));

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
    Mesh m(13);

    // add faces
    BOOST_CHECK_NO_THROW(m.add_face(0, 1, 2));
    BOOST_CHECK_NO_THROW(m.add_face(2, 1, 3));
    BOOST_CHECK_NO_THROW(m.add_face(2, 3, 4));
    BOOST_CHECK_EQUAL(m.faces.size(), 3);
    // add duplicate face
    BOOST_CHECK_NO_THROW(m.add_face(2, 3, 4));
    Face const & f0 = m.add_face(10, 11, 12);
    Face const & f1 = m.add_face(12, 10, 11);
    Face const & f2 = m.add_face(11, 12, 10);
    BOOST_CHECK_EQUAL(f0, f1);
    BOOST_CHECK_EQUAL(f0, f2);
    // one extra face, four extra vertices
    BOOST_CHECK_EQUAL(m.faces.size(), 4);
}

BOOST_AUTO_TEST_CASE(mvertex_mfaces_test_0)
{
    // construct mesh
    Mesh m(5);
    Face const & f0 = m.add_face(0, 1, 2);
    Face const & f1 = m.add_face(1, 3, 2);
    Face const & f2 = m.add_face(2, 3, 4);

    // 0->-1
    //  \ / \
    //   2-<-3
    //   2->-3
    //    \ /
    //     4

    // check face vertices
    BOOST_CHECK_EQUAL(f0.v0, 0);
    BOOST_CHECK_EQUAL(f0.v1, 1);
    BOOST_CHECK_EQUAL(f0.v2, 2);
    BOOST_CHECK_EQUAL(f1.v0, 1);
    BOOST_CHECK_EQUAL(f1.v1, 3);
    BOOST_CHECK_EQUAL(f1.v2, 2);
    BOOST_CHECK_EQUAL(f2.v0, 2);
    BOOST_CHECK_EQUAL(f2.v1, 3);
    BOOST_CHECK_EQUAL(f2.v2, 4);
    // check vertex faces
    std::set<Face> faces;
    // vertex 0
    faces.clear();
    faces.insert(f0);
    BOOST_CHECK_EQUAL(m.vertex_infos[0].faces, faces);
    // vertex 1
    faces.clear();
    faces.insert(f0);
    faces.insert(f1);
    BOOST_CHECK_EQUAL(m.vertex_infos[1].faces, faces);
    // vertex 2
    faces.clear();
    faces.insert(f0);
    faces.insert(f1);
    faces.insert(f2);
    BOOST_CHECK_EQUAL(m.vertex_infos[2].faces, faces);
    // vertex 3
    faces.clear();
    faces.insert(f1);
    faces.insert(f2);
    BOOST_CHECK_EQUAL(m.vertex_infos[3].faces, faces);
    // vertex 4
    faces.clear();
    faces.insert(f2);
    BOOST_CHECK_EQUAL(m.vertex_infos[4].faces, faces);
}

BOOST_AUTO_TEST_CASE(mvertex_faces_test_1)
{
    // construct mesh
    Mesh m(6);
    Face const & f0 = m.add_face(0, 1, 2);
    Face const & f1 = m.add_face(1, 3, 2);
    Face const & f2 = m.add_face(2, 3, 4);
    Face const & f3 = m.add_face(2, 3, 5);

    // check face vertices
    BOOST_CHECK_EQUAL(f0.v0, 0);
    BOOST_CHECK_EQUAL(f0.v1, 1);
    BOOST_CHECK_EQUAL(f0.v2, 2);
    BOOST_CHECK_EQUAL(f1.v0, 1);
    BOOST_CHECK_EQUAL(f1.v1, 3);
    BOOST_CHECK_EQUAL(f1.v2, 2);
    BOOST_CHECK_EQUAL(f2.v0, 2);
    BOOST_CHECK_EQUAL(f2.v1, 3);
    BOOST_CHECK_EQUAL(f2.v2, 4);
    BOOST_CHECK_EQUAL(f3.v0, 2);
    BOOST_CHECK_EQUAL(f3.v1, 3);
    BOOST_CHECK_EQUAL(f3.v2, 5);
    // check vertex faces
    std::set<Face> faces;
    // vertex 0
    faces.clear();
    faces.insert(f0);
    BOOST_CHECK_EQUAL(m.vertex_infos[0].faces, faces);
    // vertex 1
    faces.clear();
    faces.insert(f0);
    faces.insert(f1);
    BOOST_CHECK_EQUAL(m.vertex_infos[1].faces, faces);
    // vertex 2
    faces.clear();
    faces.insert(f0);
    faces.insert(f1);
    faces.insert(f2);
    faces.insert(f3);
    BOOST_CHECK_EQUAL(m.vertex_infos[2].faces, faces);
    // vertex 3
    faces.clear();
    faces.insert(f1);
    faces.insert(f2);
    faces.insert(f3);
    BOOST_CHECK_EQUAL(m.vertex_infos[3].faces, faces);
    // vertex 4
    faces.clear();
    faces.insert(f2);
    BOOST_CHECK_EQUAL(m.vertex_infos[4].faces, faces);
    // vertex 5
    faces.clear();
    faces.insert(f3);
    BOOST_CHECK_EQUAL(m.vertex_infos[5].faces, faces);
}

BOOST_AUTO_TEST_CASE(mvertex_faces_test_2)
{
    // single triangle mesh
    Mesh m(3);
    Face const & f = m.add_face(0, 1, 2);
    // check face vertices
    BOOST_CHECK_EQUAL(f.v0, 0);
    BOOST_CHECK_EQUAL(f.v1, 1);
    BOOST_CHECK_EQUAL(f.v2, 2);
    // check vertex faces
    std::set<Face> faces;
    faces.clear();
    faces.insert(f);
    BOOST_CHECK_EQUAL(m.vertex_infos[0].faces, faces);
    BOOST_CHECK_EQUAL(m.vertex_infos[1].faces, faces);
    BOOST_CHECK_EQUAL(m.vertex_infos[2].faces, faces);
}

BOOST_AUTO_TEST_CASE(score_mesh_empty_test)
{
    VertexScore vertex_score;
    Mesh m(0);
    BOOST_CHECK_NO_THROW(m.update_score(Mesh::VertexList(), vertex_score));
}

BOOST_AUTO_TEST_CASE(score_mesh_single_test)
{
    VertexScore vertex_score;
    Mesh m(3);
    m.add_face(0, 1, 2);
    Mesh::VertexList vertices;
    vertices.push_back(0);
    vertices.push_back(1);
    vertices.push_back(2);
    BOOST_CHECK_NO_THROW(m.update_score(vertices, vertex_score));
}

BOOST_AUTO_TEST_CASE(score_mesh_double_test)
{
    VertexScore vertex_score;
    Mesh m(4);
    m.add_face(0, 1, 2);
    m.add_face(2, 1, 3);
    Mesh::VertexList vertices;
    vertices.push_back(0);
    vertices.push_back(1);
    vertices.push_back(2);
    vertices.push_back(3);
    BOOST_CHECK_NO_THROW(m.update_score(vertices, vertex_score));
}

BOOST_AUTO_TEST_CASE(optimize_mesh_test_1)
{
    VertexScore vertex_score;
    Mesh m(10);
    Face const & f0 = m.add_face(0, 1, 2);
    Face const & f1 = m.add_face(7, 8, 9);
    Face const & f2 = m.add_face(2, 3, 4);
    std::list<Face> faces;
    faces.push_back(f1);
    faces.push_back(f0);
    faces.push_back(f2);
    BOOST_CHECK_EQUAL(faces, m.get_cache_optimized_faces(vertex_score));
}

BOOST_AUTO_TEST_SUITE_END()
