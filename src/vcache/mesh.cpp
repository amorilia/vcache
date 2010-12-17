/*

Copyright (c) 2007-2010, Python File Format Interface
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above
     copyright notice, this list of conditions and the following
     disclaimer in the documentation and/or other materials provided
     with the distribution.

   * Neither the name of the Python File Format Interface
     project nor the names of its contributors may be used to endorse
     or promote products derived from this software without specific
     prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

*/

#include <iostream> // for dump
#include <stdexcept>

#include <boost/foreach.hpp>

#include "vcache/defines.hpp"
#include "vcache/mesh.hpp"

MVertex::MVertex(int vertex)
    : vertex(vertex), cache_position(-1), score(-VCACHE_PRECISION), mfaces()
{
    // nothing to do: faces are set in Mesh::add_face.
};

void MVertex::dump() const
{
    std::cout << "  vertex " << vertex << std::endl;
    std::cout << "    score     " << score << std::endl;
    std::cout << "    cache pos " << cache_position << std::endl;
    BOOST_FOREACH(boost::weak_ptr<MFace> mface, mfaces) {
        if (MFacePtr f = mface.lock()) {
            std::cout << "    face      " << f->mv0->vertex << "," << f->mv1->vertex << "," << f->mv2->vertex << std::endl;
        };
    };
}

Face::Face(int _v0, int _v1, int _v2)
{
    // ensure it is not degenerate
    if ((_v0 == _v1) || (_v1 == _v2) || (_v0 == _v2))
        throw std::runtime_error("Degenerate face.");
    // order vertex indices
    if ((_v0 < _v1) && (_v0 < _v2)) {
        v0 = _v0;
        v1 = _v1;
        v2 = _v2;
    } else if ((_v1 < _v0) && (_v1 < _v2)) {
        v0 = _v1;
        v1 = _v2;
        v2 = _v0;
    } else if ((_v2 < _v0) && (_v2 < _v1)) {
        v0 = _v2;
        v1 = _v0;
        v2 = _v1;
    } else {
        throw std::runtime_error("Oops. Face construction bug!");
    }
};

bool Face::operator<(const Face & otherface) const
{
    if (v0 < otherface.v0) return true;
    if (v0 > otherface.v0) return false;
    if (v1 < otherface.v1) return true;
    if (v1 > otherface.v1) return false;
    if (v2 < otherface.v2) return true;
    return false;
};

bool Face::operator==(const Face & otherface) const
{
    return ((v0 == otherface.v0) && (v1 == otherface.v1) && (v2 == otherface.v2));
};

MFace::MFace()
    : mv0(), mv1(), mv2(), score(-3 * VCACHE_PRECISION)
{
    // nothing to do
};

void MFace::dump() const
{
    std::cout << "  face " << mv0->vertex << "," << mv1->vertex << "," << mv2->vertex << std::endl;
    std::cout << "    score " << score << std::endl;
}

MVertexPtr Mesh::add_vertex(MFacePtr mface, int vertex)
{
    MVertexPtr mvertex;
    // search for vertex in mesh
    VertexMap::const_iterator vertex_iter = _vertices.find(vertex);
    if (vertex_iter != _vertices.end()) {
        // vertex already exists!
        mvertex = vertex_iter->second;
    } else {
        // create vertex
        mvertex = MVertexPtr(new MVertex(vertex));
        _vertices[vertex] = mvertex;
    };
    // update list of faces that have this vertex
    mvertex->mfaces.insert(mface);
    return mvertex;
};

Mesh::Mesh() : _faces(), _vertices() {};

MFacePtr Mesh::add_face(int v0, int v1, int v2)
{
    // create face index and search if face already exists in mesh
    Face face(v0, v1, v2);
    FaceMap::const_iterator face_iter = _faces.find(face);
    if (face_iter != _faces.end()) {
        // face already exists!
        return face_iter->second;
    } else {
        // create face
        MFacePtr mface(new MFace);
        _faces[face] = mface;
        // create vertices and update links between faces and vertices
        mface->mv0 = add_vertex(mface, v0);
        mface->mv1 = add_vertex(mface, v1);
        mface->mv2 = add_vertex(mface, v2);
        return mface;
    };
}

void Mesh::dump() const
{
    std::cout << _faces.size() << " faces" << std::endl;
    BOOST_FOREACH(FaceMap::value_type face, _faces) {
        face.second->dump();
    };
    std::cout << _vertices.size() << " vertices" << std::endl;
    BOOST_FOREACH(VertexMap::value_type vertex, _vertices) {
        vertex.second->dump();
    };
}

void Mesh::update_score(VertexScore const & vertex_score)
{
    // calculate score of all vertices
    BOOST_FOREACH(VertexMap::value_type vertex, _vertices) {
        MVertexPtr mvertex = vertex.second;
        mvertex->score =
            vertex_score.get(
                mvertex->cache_position,
                mvertex->mfaces.size());
    };
    // calculate score of all triangles
    BOOST_FOREACH(FaceMap::value_type face, _faces) {
        MFacePtr mface = face.second;
        mface->score =
            mface->mv0->score +
            mface->mv1->score +
            mface->mv2->score;
    };
}

std::list<MFacePtr> Mesh::get_cache_optimized_faces(VertexScore const & vertex_score)
{
}
