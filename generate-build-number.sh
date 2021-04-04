#!/bin/bash

# version=`cat version.txt`
build=$[`git log --oneline | wc -l`]
builddate=`date +"%F" `
buildtime=`date +"%T" `
commithash=`git show -s --format=%H`

echo "Generating build $build ($builddate $buildtime)..."

# For Windows native projects
echo "// Auto-generated build information code file" > BuildInfo.h
echo "// (C) Xlfdll Workstation" >> BuildInfo.h
echo "" >> BuildInfo.h
echo "#ifndef BUILDINFO_H" >> BuildInfo.h
echo "#define BUILDINFO_H" >> BuildInfo.h
echo "" >> BuildInfo.h
echo "#define Build $build" >> BuildInfo.h
echo "#define BuildDate \"$builddate\"" >> BuildInfo.h
echo "#define BuildTime \"$buildtime\"" >> BuildInfo.h
echo "#define CommitHash \"$commithash\"" >> BuildInfo.h
echo "" >> BuildInfo.h
echo "#endif" >> BuildInfo.h

echo "Done."