#!/bin/bash

# Find all .cpp and .h files recursively in the src directory
files=$(find src -type f -name "*.cpp" -o -name "*.h")

# Format each file with clang-format and log the output only if there are errors
for file in $files
do
  formatted=$(clang-format $file)
  if [ "$formatted" != "$(cat $file)" ]; then
    echo "Formatting file: $file"
    echo "$formatted" > $file
  fi
done 2>&1 | grep -v "clang-format: error" 

echo "Done formatting files."
