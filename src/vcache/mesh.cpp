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

//#define DEBUG

#include <algorithm> // std::find
#include <boost/foreach.hpp> // BOOST_FOREACH
#include <boost/typeof/typeof.hpp> // BOOST_AUTO
#include <deque>
#include <stdexcept>

#include "vcache/defines.hpp"
#include "vcache/mesh.hpp"

std::ostream & operator<<(std::ostream & stream, VertexInfo const & vertex)
{
    stream << "VertexInfo(";
    stream << "cache_position=" << vertex.cache_position << ", ";
    stream << "valence=" << vertex.faces.size() << ", ";
    stream << "score=" << vertex.score;
    stream << ")";
    return stream;
};

std::ostream & operator<<(std::ostream & stream, Face const & face)
{
    stream << "Face(";
    stream << "v0=" << face.v0 << ", ";
    stream << "v1=" << face.v1 << ", ";
    stream << "v2=" << face.v2;
    stream << ")";
    return stream;
};

std::ostream & operator<<(std::ostream & stream, Mesh const & mesh)
{
    stream << mesh.faces.size() << " faces" << std::endl;
    BOOST_FOREACH(Face const & face, mesh.faces) {
        stream << face << std::endl;
    };
    BOOST_FOREACH(VertexInfo const & vertex_info, mesh.vertex_infos) {
        stream << vertex_info << std::endl;
    };
    return stream;
}

VertexInfo::VertexInfo()
    : cache_position(VCACHE_CACHE_SIZE), faces(), score(-VCACHE_PRECISION)
{
    // nothing to do: faces are set in Mesh::add_face.
};

void VertexInfo::update_score(VertexScore const & vertex_score)
{
    score = vertex_score.get(cache_position, faces.size());
};

Face::Face(std::size_t v0, std::size_t v1, std::size_t v2)
    : v0((v0 < v1 && v0 < v2)?v0:((v1 < v0 && v1 < v2)?v1:v2)),
      v1((v0 < v1 && v0 < v2)?v1:((v1 < v0 && v1 < v2)?v2:v0)),
      v2((v0 < v1 && v0 < v2)?v2:((v1 < v0 && v1 < v2)?v0:v1))
{
    // ensure it is not degenerate
    if ((v0 == v1) || (v1 == v2) || (v0 == v2))
        throw std::runtime_error("Degenerate face.");
};

bool Face::operator<(const Face & other) const
{
    if (v0 < other.v0) return true;
    if (v0 > other.v0) return false;
    if (v1 < other.v1) return true;
    if (v1 > other.v1) return false;
    if (v2 < other.v2) return true;
    return false;
};

bool Face::operator==(const Face & otherface) const
{
    return ((v0 == otherface.v0) && (v1 == otherface.v1) && (v2 == otherface.v2));
};

Mesh::Mesh(std::size_t num_vertices) : faces(), vertex_infos(num_vertices) {};

Face const & Mesh::add_face(std::size_t v0, std::size_t v1, std::size_t v2)
{
    // set::insert inserts a new element or returns existing element
    BOOST_AUTO(result, faces.insert(Face(v0, v1, v2)));
    if (result.second) {
        // face did not yet exist
        // so update links between faces and vertex_infos
        vertex_infos[v0].faces.insert(*(result.first));
        vertex_infos[v1].faces.insert(*(result.first));
        vertex_infos[v2].faces.insert(*(result.first));
    };
    return *(result.first);
}

std::set<Face> Mesh::update_score(
    VertexList const & vertices, VertexScore const & vertex_score)
{
    std::set<Face> updated_faces;
    // update score of all vertex_infos and keep track of faces that contain them
    BOOST_FOREACH(std::size_t vertex, vertices) {
        VertexInfo & vertex_info = vertex_infos[vertex];
        vertex_info.update_score(vertex_score);
        updated_faces.insert(
            vertex_info.faces.begin(),
            vertex_info.faces.end());
    };
    return updated_faces;
};

Face Mesh::erase_best_face(Faces const & updated_faces)
{
    if (faces.empty()) {
        throw std::runtime_error("Fatal error in erase_best_face: empty face list.");
    };
#ifdef DEBUG
    std::cout << "****** begin erase_best_face ******" << std::endl;
#endif
    int best_score = -1000 * VCACHE_PRECISION; // minus infinity
    BOOST_AUTO(best_face, updated_faces.end());
    for (BOOST_AUTO(face, updated_faces.begin()); face != updated_faces.end(); ++face) {
        int face_score =
            vertex_infos[face->v0].score +
            vertex_infos[face->v1].score +
            vertex_infos[face->v2].score;
#ifdef DEBUG
        std::cout << *face << " " << face_score << std::endl;
#endif
        if (face_score > best_score) {
            best_score = face_score;
            best_face = face;
        };
    };
    faces.erase(*best_face);
#ifdef DEBUG
    std::cout << "best face: " << *best_face << std::endl;
    std::cout << "****** end get_best_face ******" << std::endl;
#endif
    return *best_face;
}

std::list<Face> Mesh::get_cache_optimized_faces(VertexScore const & vertex_score)
{
    // result
    std::list<Face> cache_optimized_faces;
    // emulated cache
    std::deque<std::size_t> cache;
    // set of vertex_infos whose scores have to be updated in the next run
    VertexList updated_vertices;
    // for first run, all vertices need to be updated
    for (std::size_t vertex = 0; vertex < vertex_infos.size(); ++vertex) {
        updated_vertices.push_back(vertex);
    };
    // add faces by maximal score, updating the score as we go along
    while (!faces.empty()) {
        // calculate score of all vertex_infos from previous run
        Faces updated_faces = update_score(updated_vertices, vertex_score);
        // if scores of triangles were updated in the previous run
        // then restrict the search to those
        // this is suboptimal, but the difference is usually very
        // small and it is *much* faster (as noted by Forsyth)
        if (updated_faces.empty()) {
            // very slow but correct global maximum
            updated_faces = faces;
        };
        // find the best face, or a good approximation
        Face best_face(erase_best_face(updated_faces));
        // append to ordered list of triangles
        cache_optimized_faces.push_back(best_face);
        // clean lists of vertex_infos and triangles whose score we will update
        updated_vertices.clear();
        // for each vertex in the just added triangle
        std::size_t best_verts[] = {best_face.v0, best_face.v1, best_face.v2};
        BOOST_FOREACH(std::size_t const & vertex, best_verts) {
            // remove triangle from the triangle list of the vertex
            vertex_infos[vertex].faces.erase(best_face);
            // must update its score
            updated_vertices.push_back(vertex);
        };
        // add each vertex to cache, checking for cache overflows
        BOOST_FOREACH(std::size_t const & vertex, best_verts) {
            BOOST_AUTO(
                it, std::find(cache.begin(), cache.end(), vertex));
            if (it == cache.end()) {
                // vertex not in cash already: add it
                cache.push_front(vertex);
                if (cache.size() > VCACHE_CACHE_SIZE) {
                    // cache overflow!
                    // remove last vertex from cache
                    std::size_t removed_vertex = cache.back();
                    cache.pop_back();
                    // update its cache position
                    vertex_infos[removed_vertex].cache_position = VCACHE_CACHE_SIZE;
                    // must update its score
                    updated_vertices.push_back(removed_vertex);
                };
            };
        };
        // for each vertex in the cache (this includes those from the
        // just added triangle)
        std::size_t cache_pos = 0;
        BOOST_FOREACH(std::size_t const & vertex, cache) {
            // update cache positions
            vertex_infos[vertex].cache_position = cache_pos;
            // must update its score
            updated_vertices.push_back(vertex);
            ++cache_pos;
        };
    };
    // return result
    return cache_optimized_faces;
}
