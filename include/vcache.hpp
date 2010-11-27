/*
Algorithms to reorder triangle list order and vertex order aiming to
minimize vertex cache misses.

This is effectively an implementation of
'Linear-Speed Vertex Cache Optimisation' by Tom Forsyth, 28th September 2006
http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html
*/

// ***** BEGIN LICENSE BLOCK *****
//
// Copyright (c) 2007-2010, Python File Format Interface
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//
//    * Redistributions in binary form must reproduce the above
//      copyright notice, this list of conditions and the following
//      disclaimer in the documentation and/or other materials provided
//      with the distribution.
//
//    * Neither the name of the Python File Format Interface
//      project nor the names of its contributors may be used to endorse
//      or promote products derived from this software without specific
//      prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// ***** END LICENSE BLOCK *****

#ifndef VCACHE_HPP_INCLUDED
#define VCACHE_HPP_INCLUDED

#include <algorithm> // std::max_element
#include <boost/foreach.hpp> // BOOST_FOREACH
#include <cmath> // std::pow
#include <list> // std::list
#include <vector> // std::vector
#include <set> // std::set
#include <stdexcept> // std::runtime_error

#include "vcache/defines.hpp"
#include "vcache/vertex_score.hpp"

/* Stores information about a vertex. */
class VertexInfo
{
public:
    int cache_position;
    int score;
    // only triangles that have *not* yet been drawn are in this list
    std::set<int> triangle_indices;

    VertexInfo() : cache_position(-1), score(-VCACHE_PRECISION), triangle_indices() {};
};

/* Stores information about a triangle. */
class TriangleInfo
{
public:
    int score;
    std::list<int> vertex_indices;

    TriangleInfo(std::list<int> const & vertex_indices)
        : score(0), vertex_indices(vertex_indices) {
        if (vertex_indices.size() != 3) {
            throw std::runtime_error("triangle must have exactly three vertices");
        };
    };
};

/* Simple mesh implementation which keeps track of which triangles
   are used by which vertex, and vertex cache positions.
*/
class Mesh
{
public:
    std::vector<VertexInfo> vertex_infos;
    std::vector<TriangleInfo> triangle_infos;
    VertexScore vertex_score;

    /* Initialize mesh from given set of triangles. */
    Mesh(std::list<std::list<int> > const & triangles,
         VertexScore const & vertex_score)
        : vertex_infos(), triangle_infos(), vertex_score(vertex_score) {

        if (triangles.empty()) {
            // special case: no triangles, so nothing to do
            return;
        }
        // add all vertices
        int num_vertices = 0;
        BOOST_FOREACH(std::list<int> const & verts, triangles) {
            num_vertices =
                std::max(
                    num_vertices,
                    *std::max_element(verts.begin(), verts.end()));
        };
        ++num_vertices;
        vertex_infos = std::vector<VertexInfo>(num_vertices);
        // add all unique triangles
        int triangle_index = 0;
        BOOST_FOREACH(std::list<int> const & verts, triangles) {
            triangle_infos.push_back(TriangleInfo(verts));
            BOOST_FOREACH(int vertex, verts) {
                vertex_infos[vertex].triangle_indices.insert(triangle_index);
            };
            ++triangle_index;
        };
        // calculate score of all vertices
        BOOST_FOREACH(VertexInfo & vertex_info, vertex_infos) {
            vertex_info.score =
                vertex_score.get(
                    vertex_info.cache_position,
                    vertex_info.triangle_indices.size());
        };
        // calculate score of all triangles
        BOOST_FOREACH(TriangleInfo & triangle_info, triangle_infos) {
            triangle_info.score = 0;
            BOOST_FOREACH(int vertex, triangle_info.vertex_indices) {
                triangle_info.score += vertex_infos[vertex].score;
            };
        };
    };
};

/* TODO
    def get_cache_optimized_triangles(self):
        /* Reorder triangles in a cache efficient way.

        >>> m = Mesh([(0,1,2), (7,8,9),(2,3,4)])
        >>> m.get_cache_optimized_triangles()
        [(7, 8, 9), (0, 1, 2), (2, 3, 4)]
        */
/* TODO
        triangles = []
        cache = collections.deque()
        // set of vertex indices whose scores were updated in the previous run
        updated_vertices = set()
        // set of triangle indices whose scores were updated in the previous run
        updated_triangles = set()
        while (updated_triangles
               or any(triangle_info for triangle_info in self.triangle_infos)):
            // pick triangle with highest score
            if self._DEBUG or not updated_triangles:
                // very slow but correct global maximum
                best_triangle_index, best_triangle_info = max(
                    (triangle
                     for triangle in enumerate(self.triangle_infos)
                     if triangle[1]),
                    key=lambda triangle: triangle[1].score)
            if updated_triangles:
                if self._DEBUG:
                    globally_optimal_score = best_triangle_info.score
                // if scores of triangles were updated in the previous run
                // then restrict the search to those
                // this is suboptimal, but the difference is usually very small
                // and it is *much* faster (as noted by Forsyth)
                best_triangle_index = max(
                    updated_triangles,
                    key=lambda triangle_index:
                    self.triangle_infos[triangle_index].score)
                best_triangle_info = self.triangle_infos[best_triangle_index]
                if (self._DEBUG and
                    globally_optimal_score - best_triangle_info.score > 0.01):
                        print(globally_optimal_score,
                              globally_optimal_score - best_triangle_info.score,
                              len(updated_triangles))
            // mark as added
            self.triangle_infos[best_triangle_index] = None
            // append to ordered list of triangles
            triangles.append(best_triangle_info.vertex_indices)
            // clean lists of vertices and triangles whose score we will update
            updated_vertices = set()
            updated_triangles = set()
            // for each vertex in the just added triangle
            for vertex in best_triangle_info.vertex_indices:
                vertex_info = self.vertex_infos[vertex]
                // remove triangle from the triangle list of the vertex
                vertex_info.triangle_indices.remove(best_triangle_index)
                // must update its score
                updated_vertices.add(vertex)
                updated_triangles.update(vertex_info.triangle_indices)
            // add each vertex to cache (score is updated later)
            for vertex in best_triangle_info.vertex_indices:
                if vertex not in cache:
                    cache.appendleft(vertex)
                    if len(cache) > self.vertex_score.CACHE_SIZE:
                        // cache overflow!
                        // remove vertex from cache
                        removed_vertex = cache.pop()
                        removed_vertex_info = self.vertex_infos[removed_vertex]
                        // update its cache position
                        removed_vertex_info.cache_position = -1
                        // must update its score
                        updated_vertices.add(removed_vertex)
                        updated_triangles.update(removed_vertex_info.triangle_indices)
            // for each vertex in the cache (this includes those from the
            // just added triangle)
            for i, vertex in enumerate(cache):
                vertex_info = self.vertex_infos[vertex]
                // update cache positions
                vertex_info.cache_position = i
                // must update its score
                updated_vertices.add(vertex)
                updated_triangles.update(vertex_info.triangle_indices)
            // update scores
            for vertex in updated_vertices:
                self.vertex_score.update_score(self.vertex_infos[vertex])
            for triangle in updated_triangles:
                triangle_info = self.triangle_infos[triangle]
                triangle_info.score = sum(
                    self.vertex_infos[vertex].score
                    for vertex in triangle_info.vertex_indices)
        // return result
        return triangles

def get_cache_optimized_triangles(triangles):
    /* Calculate cache optimized triangles, and return the result as
    a reordered set of triangles or strip of stitched triangles.

    :param triangles: The triangles (triples of vertex indices).
    :return: A list of reordered triangles.
    */
/* TODO
    mesh = Mesh(triangles)
    return mesh.get_cache_optimized_triangles()

def get_unique_triangles(triangles):
    /* Yield unique triangles.

    >>> list(get_unique_triangles([(0, 1, 2), (1, 1, 0), (2, 1, 0), (1, 0, 0)]))
    [(0, 1, 2), (0, 2, 1)]
    >>> list(get_unique_triangles([(0, 1, 2), (1, 1, 0), (2, 0, 1)]))
    [(0, 1, 2)]
    */
/* TODO
    _added_triangles = set()
    for v0, v1, v2 in triangles:
        if v0 == v1 or v1 == v2 or v2 == v0:
            // skip degenerate triangles
            continue
        if v0 < v1 and v0 < v2:
            verts = (v0, v1, v2)
        elif v1 < v0 and v1 < v2:
            verts = (v1, v2, v0)
        elif v2 < v0 and v2 < v1:
            verts = (v2, v0, v1)
        if verts not in _added_triangles:
            yield verts
            _added_triangles.add(verts)

def stable_stripify(triangles, stitchstrips=False):
    /* Stitch all triangles together into a strip without changing the
    triangle ordering (for example because their ordering is already
    optimized).

    :param triangles: The triangles (triples of vertex indices).
    :return: A list of strips (list of vertex indices).

    >>> stable_stripify([(0, 1, 2), (2, 1, 4)])
    [[0, 1, 2, 4]]
    >>> stable_stripify([(0, 1, 2), (2, 3, 4)])
    [[0, 1, 2], [2, 3, 4]]
    >>> stable_stripify([(0, 1, 2), (2, 1, 3), (2, 3, 4), (1, 4, 5), (5, 4, 6)])
    [[0, 1, 2, 3, 4], [1, 4, 5, 6]]
    >>> stable_stripify([(0, 1, 2), (0, 3, 1), (0, 4, 3), (3, 5, 1), (6, 3, 4)])
    [[2, 0, 1, 3], [0, 4, 3], [3, 5, 1], [6, 3, 4]]
    */
/* TODO
    // all orientation preserving triangle permutations
    indices = ((0, 1, 2), (1, 2, 0), (2, 0, 1))
    // list of all strips so far
    strips = []
    // current strip that is being built
    strip = []
    // add a triangle at a time
    for tri in triangles:
        if not strip:
            // empty strip
            strip.extend(tri)
        elif len(strip) == 3:
            // strip with single triangle
            // see if we can append a vertex
            // we can rearrange the original strip as well
            added = False
            for v0, v1, v2 in indices:
                for ov0, ov1, ov2 in indices:
                    if strip[v1] == tri[ov1] and  strip[v2] == tri[ov0]:
                        strip = [strip[v0], strip[v1], strip[v2], tri[ov2]]
                        added = True
                        break
                if added:
                    // triangle added: break loop
                    break
            if added:
                // triangle added: process next triangle
                continue
            // start new strip
            strips.append(strip)
            strip = list(tri)
        else:
            // strip with multiple triangles
            added = False
            for ov0, ov1, ov2 in indices:
                if len(strip) & 1:
                    if strip[-2] == tri[ov1] and  strip[-1] == tri[ov0]:
                        strip.append(tri[ov2])
                        added = True
                        break
                else:
                    if strip[-2] == tri[ov0] and  strip[-1] == tri[ov1]:
                        strip.append(tri[ov2])
                        added = True
                        break
            if added:
                // triangle added: process next triangle
                continue
            // start new strip
            strips.append(strip)
            strip = list(tri)
    // append last strip
    strips.append(strip)
    if not stitchstrips or not strips:
        return strips
    else:
        result = reduce(lambda x, y: x + y,
                        (OrientedStrip(strip) for strip in strips))
        return [list(result)]

def stripify(triangles, stitchstrips=False):
    /* Stripify triangles, optimizing for the vertex cache. */
/* TODO
    return stable_stripify(
        get_cache_optimized_triangles(triangles),
        stitchstrips=stitchstrips)

def get_cache_optimized_vertex_map(strips):
    /* Map vertices so triangles/strips have consequetive indices.

    >>> get_cache_optimized_vertex_map([])
    []
    >>> get_cache_optimized_vertex_map([[]])
    []
    >>> get_cache_optimized_vertex_map([[0, 1, 3], []])
    [0, 1, None, 2]
    >>> get_cache_optimized_vertex_map([(5,2,1),(0,2,3)])
    [3, 2, 1, 4, None, 0]
    */
/* TODO
    if strips:
        num_vertices = max(max(strip) if strip else -1
                           for strip in strips) + 1
    else:
        num_vertices = 0
    vertex_map = [None for i in xrange(num_vertices)]
    new_vertex = 0
    for strip in strips:
        for old_vertex in strip:
            if vertex_map[old_vertex] is None:
                vertex_map[old_vertex] = new_vertex
                new_vertex += 1
    return vertex_map

def average_transform_to_vertex_ratio(strips, cache_size=16):
    /* Calculate number of transforms per vertex for a given cache size
    and triangles/strips. See
    http://castano.ludicon.com/blog/2009/01/29/acmr/
    */
/* TODO
    cache = collections.deque(maxlen=cache_size)
    // get number of vertices
    vertices = set([])
    for strip in strips:
        vertices.update(strip)
    // get number of cache misses (each miss needs a transform)
    num_misses = 0
    for strip in strips:
        for vertex in strip:
            if vertex in cache:
                pass
            else:
                cache.appendleft(vertex)
                num_misses += 1
    // return result
    if vertices:
        return num_misses / float(len(vertices))
    else:
        // no vertices...
        return 1

*/

#endif

