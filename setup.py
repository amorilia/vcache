"""Setup script for pyvcache."""

classifiers = """\
Development Status :: 4 - Beta
License :: OSI Approved :: BSD License
Intended Audience :: Developers
Intended Audience :: End Users/Desktop
Topic :: Multimedia :: Graphics :: 3D Modeling
Programming Language :: C++
Programming Language :: Cython
Programming Language :: Python
Programming Language :: Python :: 2
Programming Language :: Python :: 3
Operating System :: OS Independent"""

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext
import sys

readme = open("README.rst").read().split("\n")

setup(
    name="vcache",
    version="0.1.0",
    ext_modules=[
        Extension(
            "vcache", [
                "vcache.pyx",
                "src/vcache.cpp",
                "src/vcache/mesh.cpp",
                "src/vcache/vertex_score.cpp",
                ],
            language="c++",
            include_dirs=["include"],
            depends=[
                 "include/vcache.hpp",
                 "include/vcache/defines.hpp",
                 "include/vcache/mesh.hpp",
                 "include/vcache/vertex_score.hpp",
                 ],
            extra_compile_args = ['/EHsc'] if sys.platform == "win32" else [],
            )
        ],
    author="Amorilia",
    author_email="amorilia@users.sourceforge.net",
    license="BSD",
    keywords="vertex cache, optimization",
    platforms="any",
    description=readme[0],
    classifiers=filter(None, classifiers.split("\n")),
    long_description="\n".join(readme[2:]),
    url="http://github.com/amorilia/vcache",
    download_url="http://github.com/amorilia/vcache/downloads",
    cmdclass={'build_ext': build_ext},
)
