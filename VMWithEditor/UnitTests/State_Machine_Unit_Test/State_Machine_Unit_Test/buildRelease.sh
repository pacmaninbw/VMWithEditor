#! /bin/sh

# Creat a Release build directory and then build the target within the Release directory
# Stop on any build errors and stop the parent process.

mkdir Release
cd Release || exit
cmake -DCMAKE_BUILD_TYPE=Release ..
retVal=$?
if [ $retVal -ne 0 ]; then
    printf "\n\ncmake failed %s!\n\n" "$retVal"
    exit $retVal
fi
make
retVal=$?
if [ $retVal -ne 0 ]; then
    printf "\n\nmake failed! %s\n\n" "$retVal"
    exit $retVal
fi

