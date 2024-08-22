#!/bin/sh
find . -name "*.cpp" -o -name "*.h" | while read file; do
    clang-format -i "$file"
    echo "Formatted $file"
done