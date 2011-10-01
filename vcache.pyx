"""A python wrapper for the vcache library."""

# ***** BEGIN LICENSE BLOCK *****
#
# Copyright (c) 2007-2011, Python File Format Interface
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#
#    * Neither the name of the Python File Format Interface
#      project nor the names of its contributors may be used to endorse
#      or promote products derived from this software without specific
#      prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# ***** END LICENSE BLOCK *****

from libcpp.list cimport list as list_

cdef extern from "vcache.hpp":
    list_[list_[size_t]] c_get_cache_optimized_faces "get_cache_optimized_faces" (list_[list_[size_t]] faces)

def get_cache_optimized_faces(faces):
    """Optimize list of faces to minimize cache misses."""

    cdef list_[size_t] c_face
    cdef list_[list_[size_t]] c_faces
    cdef list_[list_[size_t]] c_opt_faces
    # convert faces to list of list of size_t's
    for face in faces:
        c_face.clear()
        for vertex in face:
            c_face.push_back(vertex)
        if c_face.size() != 3:
            raise ValueError("each face must have length 3")
        c_faces.push_back(c_face)
    # optimize
    c_opt_faces = c_get_cache_optimized_faces(c_faces)
    # convert result back to python list
    opt_faces = []
    while (not c_opt_faces.empty()):
        c_face = c_opt_faces.front()
        c_opt_faces.pop_front()
        face = []
        while (not c_face.empty()):
            face.append(c_face.front())
            c_face.pop_front()
        opt_faces.append(face)
    return opt_faces
