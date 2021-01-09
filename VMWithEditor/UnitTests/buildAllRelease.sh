#! /usr/bin/bash

# Build the debug version of all the unit tests
# Stop on any build errors.

for i in * 
do
    if [ -d $i ] ; then
        TESTDIRECTORY="$i/$i"
        SHELLFILE="$TESTDIRECTORY/buildRelease.sh";
        if [ -f $SHELLFILE ] ; then
            echo "Building $TESTDIRECTORY";
	    cd "$TESTDIRECTORY"
            ./buildRelease.sh >& buildReleaselog.txt
            retVal=$?
            if [ $retVal -ne 0 ]; then
                exit $retVal
            fi
            cd ../..
        fi
    fi
done;
