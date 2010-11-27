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

#ifndef VCACHE_SCORE_HPP_INCLUDED
#define VCACHE_SCORE_HPP_INCLUDED

#include "vcache/defines.hpp"

/* Vertex score calculation. */
class VertexScore
{
private:
    int CACHE_SCORE[VCACHE_CACHE_SIZE];
    int VALENCE_SCORE[VCACHE_VALENCE_SIZE];

public:
    VertexScore(
        float cache_decay_power=1.5,
        float last_tri_score=0.75,
        float valence_boost_scale=2.0,
        float valence_boost_power=0.5);

    /* Calculate score:

    * -1 if vertex has no triangles
    * cache score + valence score otherwise

    where cache score is

    * 0 if vertex is not in cache
    * 0.75 if vertex has been used very recently
      (position 0, 1, or 2)
    * (1 - (cache position - 3) / (32 - 3)) ** 1.5
      otherwise

    and valence score is 2 * (num triangles ** (-0.5))

    */
    int get(int cache_position, int valence) const;
};

#endif

