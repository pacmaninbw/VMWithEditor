#! /usr/bin/bash

# Build the release version of the Virtual Machine and all the unit tests
# Stop on any build errors.

UNITTESTDIRECTORY="./VMWithEditor/UnitTests"
SHELLFILE="$UNITTESTDIRECTORY/buildAllRelease.sh";
if [ -d "$UNITTESTDIRECTORY" ] ; then
    if [ -f "$SHELLFILE" ] ; then
        echo "Building $UNITTESTDIRECTORY";
	cd "$UNITTESTDIRECTORY" || exit
        ./buildAllRelease.sh >& buildReleaselog.txt
        retVal=$?
        if [ $retVal -ne 0 ]; then
            exit $retVal
        fi
        cd ../ || exit
    fi
    if [ -f "./buildRelease.sh" ] ; then
        ./buildRelease.sh
    fi
fi
