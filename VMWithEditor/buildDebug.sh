#! /bin/sh

# Creat a Debug build directory and then build the Virtual Machine with Editor
# within the Debug directory.
# Stop on any build errors and stop the parent process.

mkdir Debug
cd Debug || exit
echo "Building Debug version in "; pwd
cmake -DCMAKE_BUILD_TYPE=Debug ..
retVal=$?
if [ $retVal -ne 0 ]; then
    printf "\n\ncmake failed %s!\n\n" "$retVal"
    exit $retVal
fi
make VERBOSE=1
retVal=$?
if [ $retVal -ne 0 ]; then
    printf "\n\nmake failed! %s\n\n" "$retVal"
    exit $retVal
fi
