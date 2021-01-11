#! /usr/bin/bash
#
# Build the release version of the Virtual Machine and all the unit tests
# Stop on any build errors.
#
UNITTESTDIRECTORY="./VMWithEditor/UnitTests"
if [ -d "$UNITTESTDIRECTORY" ] ; then
    cd "$UNITTESTDIRECTORY" || exit
    make clean
    retVal=$?
    if [ $retVal -ne 0 ]; then
        exit $retVal
    fi
    cd ../ || exit
    make clean
fi
