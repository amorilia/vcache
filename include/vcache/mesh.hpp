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

#include <iostream>
#include <list>
#include <set>
#include <vector>

#include "vcache/vertex_score.hpp"

// forward declarations
class VertexInfo;
class Face;
class Mesh;

std::ostream & operator<<(std::ostream & stream, VertexInfo const & vertex);
std::ostream & operator<<(std::ostream & stream, Face const & face);
std::ostream & operator<<(std::ostream & stream, Mesh const & mesh);

//! A vertex with links to other parts of a mesh.
class VertexInfo
{
public:
    //! Position of the vertex in the cache.
    std::size_t cache_position;
    //! Only faces that have *not* yet been drawn are in this list
    //! Note: faces are set in Mesh::add_face.
    std::set<Face> faces;
    //! Score as calculated by VertexScore from cache position and
    //! size of faces.
    int score;

    //! Note: don't call directly! Use Mesh::add_face.
    VertexInfo();
    //! Update the score. Call this whenever cache position or faces change.
    void update_score(VertexScore const & vertex_score);
};

//! A non-degenerate face with links to other parts of a mesh.
//! The vertices are ordered such that v0 has the lowest index.
class Face
{
public:
    //! Note: don't call directly! Use Mesh::add_face.
    Face(std::size_t _v0, std::size_t _v1, std::size_t _v2);
    //! Strict inequality by vertex index.
    bool operator<(Face const & other) const;
    //! Equality by vertex index.
    bool operator==(Face const & other) const;
    //! First Vertex.
    std::size_t const v0;
    //! Second Vertex.
    std::size_t const v1;
    //! Third Vertex.
    std::size_t const v2;
};

//! A mesh built from faces.
class Mesh
{
public:
    // Using vector of vertices to allow random access.
    typedef std::vector<VertexInfo> VertexInfos;
    // Using set to avoid duplicate entries (no FaceInfo needed, score is
    // calculated on the fly).
    typedef std::set<Face> Faces;
    // List of vertices. Used to update score of parts of the mesh.
    typedef std::list<std::size_t> VertexList;

    //! Faces of the mesh.
    Faces faces;

    //! Vertices and vertex infos of the mesh.
    VertexInfos vertex_infos;

    //! Initialize empty mesh.
    Mesh(std::size_t num_vertices);

    //! Create new face for mesh, or return existing face.
    Face const & add_face(std::size_t v0, std::size_t v1, std::size_t v2);

    //! Update score of given vertices, and return set of affected faces.
    Faces update_score(
        VertexList const & vertices, VertexScore const & vertex_score);

    //! Erase best face.
    Face erase_best_face(Faces const & updated_faces);

    //! Calculate optimal ordering for the given vertex scoring algorithm.
    std::list<Face> get_cache_optimized_faces(VertexScore const & vertex_score);
};

#endif
