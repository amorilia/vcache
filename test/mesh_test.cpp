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
    BOOST_CHECK_EQUAL(m._faces.size(), 3);
    BOOST_CHECK_EQUAL(m._vertices.size(), 5);
    // add duplicate face
    BOOST_CHECK_NO_THROW(m.add_face(2, 3, 4));
    MFacePtr f0 = m.add_face(10, 11, 12);
    MFacePtr f1 = m.add_face(12, 10, 11);
    MFacePtr f2 = m.add_face(11, 12, 10);
    BOOST_CHECK_EQUAL(f0, f1);
    BOOST_CHECK_EQUAL(f0, f2);
    // one extra face, four extra vertices
    BOOST_CHECK_EQUAL(m._faces.size(), 4);
    BOOST_CHECK_EQUAL(m._vertices.size(), 8);
}

BOOST_AUTO_TEST_CASE(mvertex_mfaces_test_0)
{
    // construct mesh
    Mesh m;
    MFacePtr f0 = m.add_face(0, 1, 2);
    MFacePtr f1 = m.add_face(1, 3, 2);
    MFacePtr f2 = m.add_face(2, 3, 4);
    MVertexPtr v0 = m._vertices[0];
    MVertexPtr v1 = m._vertices[1];
    MVertexPtr v2 = m._vertices[2];
    MVertexPtr v3 = m._vertices[3];
    MVertexPtr v4 = m._vertices[4];

    // 0->-1
    //  \ / \
    //   2-<-3
    //   2->-3
    //    \ /
    //     4

    // check vertices
    BOOST_CHECK_EQUAL(v0->vertex, 0);
    BOOST_CHECK_EQUAL(v1->vertex, 1);
    BOOST_CHECK_EQUAL(v2->vertex, 2);
    BOOST_CHECK_EQUAL(v3->vertex, 3);
    BOOST_CHECK_EQUAL(v4->vertex, 4);
    // check face vertices
    BOOST_CHECK_EQUAL(f0->mv0, v0);
    BOOST_CHECK_EQUAL(f0->mv1, v1);
    BOOST_CHECK_EQUAL(f0->mv2, v2);
    BOOST_CHECK_EQUAL(f1->mv0, v1);
    BOOST_CHECK_EQUAL(f1->mv1, v3);
    BOOST_CHECK_EQUAL(f1->mv2, v2);
    BOOST_CHECK_EQUAL(f2->mv0, v2);
    BOOST_CHECK_EQUAL(f2->mv1, v3);
    BOOST_CHECK_EQUAL(f2->mv2, v4);
    // check vertex faces
    std::set<MFacePtr> mfaces;
    // vertex 0
    mfaces.clear();
    mfaces.insert(f0);
    BOOST_CHECK_EQUAL(lock_set(v0->mfaces), mfaces);
    // vertex 1
    mfaces.clear();
    mfaces.insert(f0);
    mfaces.insert(f1);
    BOOST_CHECK_EQUAL(lock_set(v1->mfaces), mfaces);
    // vertex 2
    mfaces.clear();
    mfaces.insert(f0);
    mfaces.insert(f1);
    mfaces.insert(f2);
    BOOST_CHECK_EQUAL(lock_set(v2->mfaces), mfaces);
    // vertex 3
    mfaces.clear();
    mfaces.insert(f1);
    mfaces.insert(f2);
    BOOST_CHECK_EQUAL(lock_set(v3->mfaces), mfaces);
    // vertex 4
    mfaces.clear();
    mfaces.insert(f2);
    BOOST_CHECK_EQUAL(lock_set(v4->mfaces), mfaces);
}

BOOST_AUTO_TEST_CASE(mvertex_mfaces_test_1)
{
    // construct mesh
    Mesh m;
    MFacePtr f0 = m.add_face(0, 1, 2);
    MFacePtr f1 = m.add_face(1, 3, 2);
    MFacePtr f2 = m.add_face(2, 3, 4);
    MFacePtr f3 = m.add_face(2, 3, 5);
    MVertexPtr v0 = m._vertices[0];
    MVertexPtr v1 = m._vertices[1];
    MVertexPtr v2 = m._vertices[2];
    MVertexPtr v3 = m._vertices[3];
    MVertexPtr v4 = m._vertices[4];
    MVertexPtr v5 = m._vertices[5];

    // check vertices
    BOOST_CHECK_EQUAL(v0->vertex, 0);
    BOOST_CHECK_EQUAL(v1->vertex, 1);
    BOOST_CHECK_EQUAL(v2->vertex, 2);
    BOOST_CHECK_EQUAL(v3->vertex, 3);
    BOOST_CHECK_EQUAL(v4->vertex, 4);
    BOOST_CHECK_EQUAL(v5->vertex, 5);
    // check face vertices
    BOOST_CHECK_EQUAL(f0->mv0, v0);
    BOOST_CHECK_EQUAL(f0->mv1, v1);
    BOOST_CHECK_EQUAL(f0->mv2, v2);
    BOOST_CHECK_EQUAL(f1->mv0, v1);
    BOOST_CHECK_EQUAL(f1->mv1, v3);
    BOOST_CHECK_EQUAL(f1->mv2, v2);
    BOOST_CHECK_EQUAL(f2->mv0, v2);
    BOOST_CHECK_EQUAL(f2->mv1, v3);
    BOOST_CHECK_EQUAL(f2->mv2, v4);
    BOOST_CHECK_EQUAL(f3->mv0, v2);
    BOOST_CHECK_EQUAL(f3->mv1, v3);
    BOOST_CHECK_EQUAL(f3->mv2, v5);
    // check vertex faces
    std::set<MFacePtr> mfaces;
    // vertex 0
    mfaces.clear();
    mfaces.insert(f0);
    BOOST_CHECK_EQUAL(lock_set(v0->mfaces), mfaces);
    // vertex 1
    mfaces.clear();
    mfaces.insert(f0);
    mfaces.insert(f1);
    BOOST_CHECK_EQUAL(lock_set(v1->mfaces), mfaces);
    // vertex 2
    mfaces.clear();
    mfaces.insert(f0);
    mfaces.insert(f1);
    mfaces.insert(f2);
    mfaces.insert(f3);
    BOOST_CHECK_EQUAL(lock_set(v2->mfaces), mfaces);
    // vertex 3
    mfaces.clear();
    mfaces.insert(f1);
    mfaces.insert(f2);
    mfaces.insert(f3);
    BOOST_CHECK_EQUAL(lock_set(v3->mfaces), mfaces);
    // vertex 4
    mfaces.clear();
    mfaces.insert(f2);
    BOOST_CHECK_EQUAL(lock_set(v4->mfaces), mfaces);
    // vertex 5
    mfaces.clear();
    mfaces.insert(f3);
    BOOST_CHECK_EQUAL(lock_set(v5->mfaces), mfaces);
}

BOOST_AUTO_TEST_CASE(mvertex_mfaces_test_2)
{
    // single triangle mesh
    Mesh m;
    MFacePtr f = m.add_face(0, 1, 2);
    MVertexPtr v0 = m._vertices[0];
    MVertexPtr v1 = m._vertices[1];
    MVertexPtr v2 = m._vertices[2];
    // check vertices
    BOOST_CHECK_EQUAL(v0->vertex, 0);
    BOOST_CHECK_EQUAL(v1->vertex, 1);
    BOOST_CHECK_EQUAL(v2->vertex, 2);
    // check face vertices
    BOOST_CHECK_EQUAL(f->mv0, v0);
    BOOST_CHECK_EQUAL(f->mv1, v1);
    BOOST_CHECK_EQUAL(f->mv2, v2);
    // check vertex faces
    std::set<MFacePtr> mfaces;
    mfaces.clear();
    mfaces.insert(f);
    BOOST_CHECK_EQUAL(lock_set(v0->mfaces), mfaces);
    BOOST_CHECK_EQUAL(lock_set(v1->mfaces), mfaces);
    BOOST_CHECK_EQUAL(lock_set(v2->mfaces), mfaces);
}

BOOST_AUTO_TEST_CASE(score_mesh_empty_test)
{
    VertexScore vertex_score;
    Mesh m;
    BOOST_CHECK_NO_THROW(m.update_score(vertex_score));
}

BOOST_AUTO_TEST_CASE(score_mesh_single_test)
{
    VertexScore vertex_score;
    Mesh m;
    m.add_face(0, 1, 2);
    BOOST_CHECK_NO_THROW(m.update_score(vertex_score));
}

BOOST_AUTO_TEST_CASE(score_mesh_double_test)
{
    VertexScore vertex_score;
    Mesh m;
    m.add_face(0, 1, 2);
    m.add_face(2, 1, 3);
    BOOST_CHECK_NO_THROW(m.update_score(vertex_score));
}

BOOST_AUTO_TEST_CASE(optimize_mesh_test_1)
{
    VertexScore vertex_score;
    Mesh m;
    MFacePtr f0 = m.add_face(0, 1, 2);
    MFacePtr f1 = m.add_face(7, 8, 9);
    MFacePtr f2 = m.add_face(2, 3, 4);
    BOOST_CHECK_NO_THROW(m.get_cache_optimized_faces(vertex_score));

    std::list<MFacePtr> faces;
    faces.push_back(f1);
    faces.push_back(f0);
    faces.push_back(f2);
    BOOST_CHECK_EQUAL(faces, m.get_cache_optimized_faces(vertex_score));
}

BOOST_AUTO_TEST_SUITE_END()
