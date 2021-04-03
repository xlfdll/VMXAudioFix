#!/bin/bash

# version=`cat version.txt`
build=$[`git log --oneline | wc -l`]
builddate=`date +"%F" `
buildtime=`date +"%T" `
commithash=`git show -s --format=%H`

echo "Generating build $build ($builddate $buildtime)..."

# For .NET Framework projects
# echo "// Auto-generated build information code file" > BuildInfo.cs
# echo "// (C) Xlfdll Workstation" >> BuildInfo.cs
# echo "" >> BuildInfo.cs
# echo "using System;" >> BuildInfo.cs
# echo "" >> BuildInfo.cs
# echo "namespace Xlfdll" >> BuildInfo.cs
# echo "{" >> BuildInfo.cs
# echo "    public static class BuildInfo" >> BuildInfo.cs
# echo "    {" >> BuildInfo.cs
# echo "        public const String Version = \"$buildversion\";" >> BuildInfo.cs
# echo "        public const String Build = \"$build\";" >> BuildInfo.cs
# echo "        public const String BuildDate = \"$builddate\";" >> BuildInfo.cs
# echo "        public const String BuildTime = \"$buildtime\";" >> BuildInfo.cs
# echo "        public const String CommitHash = \"$commithash\";" >> BuildInfo.cs
# echo "    }" >> BuildInfo.cs
# echo "}" >> BuildInfo.cs

# For .NET Core projects
# echo "<!-- Auto-generated build information code file -->" > Directory.Build.props
# echo "<!-- (C) Xlfdll Workstation -->" >> Directory.Build.props
# echo "" >> Directory.Build.props
# echo "<Project>" >> Directory.Build.props
# echo "  <PropertyGroup>" >> Directory.Build.props
# echo "    <Version>$version.$build</Version>" >> Directory.Build.props
# echo "  </PropertyGroup>" >> Directory.Build.props
# echo "</Project>" >> Directory.Build.props

# For C projects
# echo "// Auto-generated build information code file" > BuildInfo.c
# echo "// (C) Xlfdll Workstation" >> BuildInfo.c
# echo "" >> BuildInfo.c
# echo "const char* Version = \"$buildversion\";" >> BuildInfo.c
# echo "const char* Build = \"$build\";" >> BuildInfo.c
# echo "const char* BuildDate = \"$builddate\";" >> BuildInfo.c
# echo "const char* BuildTime = \"$buildtime\";" >> BuildInfo.c
# echo "const char* CommitHash = \"$commithash\";" >> BuildInfo.c

# For C++ projects
# -- Declaration
echo "// Auto-generated build information code file" > BuildInfo.hpp
echo "// (C) Xlfdll Workstation" >> BuildInfo.hpp
echo "" >> BuildInfo.hpp
echo "#ifndef BUILDINFO_HPP" >> BuildInfo.hpp
echo "#define BUILDINFO_HPP" >> BuildInfo.hpp
echo "" >> BuildInfo.hpp
echo "namespace Xlfdll" >> BuildInfo.hpp
echo "{" >> BuildInfo.hpp
echo "    class BuildInfo" >> BuildInfo.hpp
echo "    {" >> BuildInfo.hpp 
echo "    public:" >> BuildInfo.hpp
echo "        static const char* Version;" >> BuildInfo.hpp
echo "        static const char* Build;" >> BuildInfo.hpp
echo "        static const char* BuildDate;" >> BuildInfo.hpp
echo "        static const char* BuildTime;" >> BuildInfo.hpp
echo "        static const char* CommitHash;" >> BuildInfo.hpp
echo "    };" >> BuildInfo.hpp
echo "}" >> BuildInfo.hpp
echo "" >> BuildInfo.hpp
echo "#endif" >> BuildInfo.hpp
# -- Implementation
echo "// Auto-generated build information code file" > BuildInfo.cpp
echo "// (C) Xlfdll Workstation" >> BuildInfo.cpp
echo "" >> BuildInfo.cpp
echo "#include \"BuildInfo.hpp\"" >> BuildInfo.cpp
echo "" >> BuildInfo.cpp
echo "namespace Xlfdll" >> BuildInfo.cpp
echo "{" >> BuildInfo.cpp
echo "    const char* BuildInfo::Version = \"$buildversion\";" >> BuildInfo.cpp
echo "    const char* BuildInfo::Build = \"$build\";" >> BuildInfo.cpp
echo "    const char* BuildInfo::BuildDate = \"$builddate\";" >> BuildInfo.cpp
echo "    const char* BuildInfo::BuildTime = \"$buildtime\";" >> BuildInfo.cpp
echo "    const char* BuildInfo::CommitHash = \"$commithash\";" >> BuildInfo.cpp
echo "}" >> BuildInfo.cpp

echo "Done."