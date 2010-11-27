#!/bin/sh
find . \( -name "*.cpp" -or -name "*.hpp" \) -exec astyle --style=linux -s {} \;
find . -name "*.orig" -exec rm {} \;
