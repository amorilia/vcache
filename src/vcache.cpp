/*

Copyright (c) 2007-2009, Python File Format Interface
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

std::list<std::list<int> > get_cache_optimized_faces(std::list<std::list<int> > const & faces, VertexScore const & vertex_score)
{
    // get maximal vertex index
    Vertex num_vertices = 0;
    BOOST_FOREACH(std::list<int> const & face, faces) {
        BOOST_FOREACH(int const & vertex, face) {
            if (num_vertices <= vertex) {
                num_vertices = vertex + 1;
            };
        };
    };
    // initialize mesh
    Mesh mesh(num_vertices);
    BOOST_FOREACH(std::list<int> const & face, faces) {
        std::list<int>::const_iterator iter = face.begin();
        int v0 = *iter;
        int v1 = *(++iter);
        int v2 = *(++iter);
        mesh.add_face(v0, v1, v2);
    };
    // find optimal ordering
    std::list<std::list<int> > result;
    BOOST_FOREACH(Face const & mface, mesh.get_cache_optimized_faces(vertex_score)) {
        std::list<int> face;
        face.push_back(mface.v0);
        face.push_back(mface.v1);
        face.push_back(mface.v2);
        result.push_back(face);
    };
    return result;
}

float get_average_transform_to_vertex_ratio(std::list<std::list<int> > faces, int cache_size)
{
    std::deque<int> cache;
    // get number of vertices
    std::set<int> vertices;
    BOOST_FOREACH(std::list<int> const & face, faces) {
        vertices.insert(face.begin(), face.end());
    };
    // get number of cache misses (each miss needs a transform)
    int num_misses = 0;
    BOOST_FOREACH(std::list<int> const & face, faces) {
        BOOST_FOREACH(int const & vertex, face) {
            std::deque<int>::const_iterator iter = std::find(cache.begin(), cache.end(), vertex);
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
    if (vertices.empty()) {
        return 1;
    } else {
        return num_misses / float(vertices.size());
    };
}
