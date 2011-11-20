/*

Copyright (c) 2007-2011, Python File Format Interface
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

#include <deque>
#include <list>

#include "vcache.hpp"

std::list<std::list<std::size_t> > get_cache_optimized_faces(std::list<std::list<std::size_t> > const & faces, VertexScore const & vertex_score)
{
    // get maximal vertex index
    std::size_t num_vertices = 0;
    BOOST_FOREACH(std::list<std::size_t> const & face, faces) {
        BOOST_FOREACH(std::size_t const & vertex, face) {
            if (num_vertices <= vertex) {
                num_vertices = vertex + 1;
            };
        };
    };
    // initialize mesh
    Mesh mesh(num_vertices);
    BOOST_FOREACH(std::list<std::size_t> const & face, faces) {
        if (face.size() != 3) {
            throw std::runtime_error("each face must have size 3");
        };
        std::list<std::size_t>::const_iterator iter = face.begin();
        std::size_t v0 = *iter;
        std::size_t v1 = *(++iter);
        std::size_t v2 = *(++iter);
        // only add degenerate faces
        if (v0 != v1 && v1 != v2 && v2 != v0) {
            mesh.add_face(v0, v1, v2);
        };
    };
    // find optimal ordering
    std::list<std::list<std::size_t> > result;
    BOOST_FOREACH(Face const & mface, mesh.get_cache_optimized_faces(vertex_score)) {
        std::list<std::size_t> face;
        face.push_back(mface.v0);
        face.push_back(mface.v1);
        face.push_back(mface.v2);
        result.push_back(face);
    };
    return result;
}

std::pair<std::size_t, std::size_t> get_transform_to_vertex_ratio(std::list<std::list<std::size_t> > faces, std::size_t cache_size)
{
    std::deque<std::size_t> cache;
    // get number of vertices
    std::set<std::size_t> vertices;
    BOOST_FOREACH(std::list<std::size_t> const & face, faces) {
        vertices.insert(face.begin(), face.end());
    };
    // get number of cache misses (each miss needs a transform)
    std::size_t num_misses = 0;
    BOOST_FOREACH(std::list<std::size_t> const & face, faces) {
        BOOST_FOREACH(std::size_t const & vertex, face) {
            std::deque<std::size_t>::const_iterator iter = std::find(cache.begin(), cache.end(), vertex);
            if (iter == cache.end()) {
                cache.push_front(vertex);
                if (cache.size() > cache_size) {
                    cache.pop_back();
                };
                ++num_misses;
            };
        };
    };
    // return result
    return std::pair<std::size_t, std::size_t>(num_misses, vertices.size());
}
