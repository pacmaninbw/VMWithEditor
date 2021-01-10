#! /usr/bin/bash

# Build the debug version of the Virtual MAchine and all the unit tests
# Stop on any build errors.

UNITTESTDIRECTORY="./VMWithEditor/UnitTests"
SHELLFILE="$UNITTESTDIRECTORY/buildAllDebug.sh";
if [ -d "$UNITTESTDIRECTORY" ] ; then
    if [ -f "$SHELLFILE" ] ; then
        echo "Building $UNITTESTDIRECTORY";
	cd "$UNITTESTDIRECTORY" || exit
        ./buildAllDebug.sh >& buildDebuglog.txt
        retVal=$?
        if [ $retVal -ne 0 ]; then
            exit $retVal
        fi
        cd ../ || exit
    fi
    if [ -f "./buildDebug.sh" ] ; then
        ./buildDebug.sh
    fi
fi
