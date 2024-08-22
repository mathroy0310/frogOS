#!/bin/sh
if ! command -v clang-format &> /dev/null
then
    echo "clang-format could not be found. Please install it first."
    exit 1
fi

find . -name "*.cpp" -o -name "*.h" | while read file; do
    clang-format -i "$file"
    echo "Formatted $file"
done