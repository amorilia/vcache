/*! \mainpage vcache Documentation

The vcache library finds triangle list orderings which minimize vertex
cache misses. The implementation is based on Tom Forsyth's
<em>Linear-Speed Vertex Cache Optimisation</em> article (28th
September 2006). See
http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html
for a detailed description and discussion of the algorithm.

\todo Describe basic usage.

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
#include "vcache/mesh.hpp"

/*! Calculate number of transforms per vertex for a given cache size
 *  and triangles/strips. See
 *  http://castano.ludicon.com/blog/2009/01/29/acmr/
 */
int average_transform_to_vertex_ratio(std::list<std::list<int> > faces, int cache_size=VCACHE_CACHE_SIZE);

#endif

