#! /usr/bin/sh
#
# Build the input version of the Virtual MAchine and all the unit tests
# Stop on any build errors.
#
if [ -z "$1" ] ; then
    echo "Usage: build.sh BUILDTYPE where BUILDTYPE is Debug or Release."
    exit 1
elif [ "$1" != 'Debug' ] && [ "$1" != 'Release' ] ; then
    printf "\n unknow build type %s \n" "$1"
    exit 1
fi
#
# Build the necessary variables
#
BUILDTYPE="$1"
UNITTESTDIRECTORY="./VMWithEditor/UnitTests"
SHELLFILE="buildAll${BUILDTYPE}.sh";
VMSHELLFILE="build${BUILDTYPE}.sh";
FULLSPECSHELLFILE="${UNITTESTDIRECTORY}/${SHELLFILE}";
LOGFILE="build${BUILDTYPE}log.txt"
#
# Execute the build scripts
#
# Build The Unit Tests
#
if [ -d "${UNITTESTDIRECTORY}" ] ; then
    if [ -f "${FULLSPECSHELLFILE}" ] ; then
        echo "Building $UNITTESTDIRECTORY";
	cd "${UNITTESTDIRECTORY}" || exit
        ./"${SHELLFILE}" > "${LOGFILE}" 2>&1 
        retVal=$?
        if [ $retVal -ne 0 ]; then
            echo "Unit Test Build Failed!"
            exit $retVal
        fi
        cd ../ || exit
    fi
#
# Build the Virtual Machine with Editor
#
    if [ -f "./buildDebug.sh" ] ; then
        ./"${VMSHELLFILE}" > "${LOGFILE}" 2>&1
        retVal=$?
        if [ ${retVal} -ne 0 ]; then
            echo "Virtual Machine With Editor Build Failed!"
            echo "Check logs for details"
            exit ${retVal}
        else
            printf "%s Version Virtual Machine With Editor Build and Unit Test Build Completed!\n" "${BUILDTYPE}"
            exit 0
        fi
    fi
fi
