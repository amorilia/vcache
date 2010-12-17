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

#ifndef VCACHE_TRIANGLEMESH_HPP_INCLUDED
#define VCACHE_TRIANGLEMESH_HPP_INCLUDED

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <vector>

#include "vcache/vertex_score.hpp"

// forward declarations

class MVertex;
typedef boost::shared_ptr<MVertex> MVertexPtr;

class MFace;
typedef boost::shared_ptr<MFace> MFacePtr;

//! A vertex with links to other parts of a mesh.
class MVertex
{
public:
    int vertex;
    int cache_position;
    int score;

    typedef std::set<boost::weak_ptr<MFace> > MFaces;

    // only triangles that have *not* yet been drawn are in this list
    MFaces mfaces; //! Note: faces are set in Mesh::add_face.

    //! Note: don't call directly! Use Mesh::add_face.
    MVertex(int vertex);

    //! Dump to std::cout (e.g. for debugging).
    void dump() const;
};

//! A standalone non-degenerate oriented face.
class Face
{
public:
    //! Vertex indices, with v0 always being the lowest index.
    int v0, v1, v2;

    Face(int _v0, int _v1, int _v2);

    bool operator<(const Face & otherface) const;
    bool operator==(const Face & otherface) const;
};

//! A non-degenerate face with links to other parts of a mesh.
class MFace
{
public:
    //! First Vertex.
    MVertexPtr mv0;
    //! Second Vertex.
    MVertexPtr mv1;
    //! Third Vertex.
    MVertexPtr mv2;
    //! Face score (sum of vertex scores).
    int score;

    //! Note: don't call directly! Use Mesh::add_face.
    MFace();

    //! Dump to std::cout (e.g. for debugging).
    void dump() const;
};

//! A mesh built from faces.
class Mesh
{
private:
    //! Create new vertex for mesh for given face, or return
    //! existing vertex. Lists of faces of the new/existing vertex
    //! is also updated. For internal use only, called on each
    //! vertex of the face in add_face.
    MVertexPtr add_vertex(MFacePtr mface, int vertex);

public:
    // We use maps to avoid duplicate entries and quickly detect
    // adjacent faces.

    typedef std::map<Face, MFacePtr> FaceMap;
    typedef std::map<int, MVertexPtr> VertexMap;

    //! Map for mesh faces. Used internally to avoid
    //! duplicates. Deleted when mesh is locked.
    FaceMap _faces;

    //! Map for mesh vertices. Used internally to build lists of
    //! vertices. Deleted when mesh is locked.
    VertexMap _vertices;

    //! Initialize empty mesh.
    Mesh();

    //! Create new face for mesh, or return existing face.
    MFacePtr add_face(int v0, int v1, int v2);

    //! Initialize score of all vertices and all faces.
    void update_score(VertexScore const & vertex_score);

    //! Calculate optimal ordering for the given vertex scoring algorithm.
    std::list<MFacePtr> get_cache_optimized_faces(VertexScore const & vertex_score);

    //! Dump to std::cout (e.g. for debugging).
    void dump() const;
};

#endif
