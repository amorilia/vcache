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

#include <cmath> // std::pow
#include <stdexcept> // std::runtime_error

#include "vcache/vertex_score.hpp"

VertexScore::VertexScore(
    float cache_decay_power,
    float last_tri_score,
    float valence_boost_scale,
    float valence_boost_power)
    : CACHE_SCORE(),
      VALENCE_SCORE()
{
    for (std::size_t cache_position = 0; cache_position < VCACHE_CACHE_SIZE; cache_position++) {
        if (cache_position < 3) {
            CACHE_SCORE[cache_position] =
                0.5 + VCACHE_PRECISION * last_tri_score;
        } else {
            CACHE_SCORE[cache_position] =
                0.5 + VCACHE_PRECISION * std::pow(
                    float(VCACHE_CACHE_SIZE - cache_position) / (VCACHE_CACHE_SIZE - 3),
                    cache_decay_power);
        };
    };
    for (std::size_t valence = 0; valence < VCACHE_VALENCE_SIZE; valence++) {
        if (valence == 0) {
            VALENCE_SCORE[valence] = 0;
        } else {
            VALENCE_SCORE[valence] =
                0.5 + VCACHE_PRECISION * valence_boost_scale * std::pow(
                    valence, -valence_boost_power);
        };
    };
};

int VertexScore::get(std::size_t cache_position, std::size_t valence) const
{
    if (valence == 0) {
        // no triangle needs this vertex
        return -VCACHE_PRECISION;
    } else {
        if (cache_position >= VCACHE_CACHE_SIZE) {
            // not in cache, so only valence score
            return VALENCE_SCORE[valence];
        } else if (valence >= VCACHE_VALENCE_SIZE) {
            // if vertex has an insane number of triangles then
            // its valence score is approximately zero anyway, so
            // only cache score (note that this will probably NEVER happen)
            return CACHE_SCORE[cache_position];
        } else {
            // in cache, and has triangles, so return both scores
            return CACHE_SCORE[cache_position] + VALENCE_SCORE[valence];
        };
    };
};
