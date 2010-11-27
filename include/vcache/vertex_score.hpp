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

//! Vertex score calculation, with customizable parameters.
class VertexScore
{
private:
    int CACHE_SCORE[VCACHE_CACHE_SIZE]; //!< Cache score lookup table.
    int VALENCE_SCORE[VCACHE_VALENCE_SIZE]; //!< Valence score lookup table.

public:
    //! Initialize scoring algorithm with given parameters.
    /*!

      \param cache_decay_power Determines how fast cache score decays
           as the cache position increases.

      \param last_tri_score Cache score assigned to each of the first
          three vertices in the cache. Should be less than 1 in order
          to stimulate the algorithm to look ahead.

      \param valence_boost_scale Weighs valence score relative to the
          cache score.

      \param valence_boost_power Determines how fast the valence score
          decays as the valence increases.
     */
    VertexScore(
        float cache_decay_power=1.5,
        float last_tri_score=0.75,
        float valence_boost_scale=2.0,
        float valence_boost_power=0.5);

    //! Calculate vertex score.
    /*!
      \param cache_position The position of the vertex in the cache. A
          value -1 means that the vertex is not in the cache, 0 for
          the first position, 1 for the second position, etc.

      \param valence The number of triangles which have this vertex
          and that still have to be drawn.

      \throws std::runtime_error When cache_position >=
          VCACHE_CACHE_SIZE or valence < 0.
      
      The score is calculated as follows (assuming default parameter
      values):

      - -1 if vertex has no triangles
      - cache score + valence score otherwise

      where cache score is

      - 0 if vertex is not in cache
      - 0.75 if vertex has been used very recently
        (position 0, 1, or 2)
      - (1 - (cache position - 3) / (32 - 3)) ** 1.5
        otherwise

      and valence score is 2 * (valence ** (-0.5)). The score is
      finally multiplied by VCACHE_PRECISION (which is 1000 by
      default), and rounded to the nearest integer.
    */
    int get(int cache_position, int valence) const;
};

#endif

