# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.3.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.3.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Kyle/Desktop/workspace/vkc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Kyle/Desktop/workspace/vkc/build

# Include any dependencies generated for this target.
include CMakeFiles/vkc.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/vkc.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vkc.dir/flags.make

CMakeFiles/vkc.dir/main.cpp.o: CMakeFiles/vkc.dir/flags.make
CMakeFiles/vkc.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/Kyle/Desktop/workspace/vkc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/vkc.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vkc.dir/main.cpp.o -c /Users/Kyle/Desktop/workspace/vkc/main.cpp

CMakeFiles/vkc.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vkc.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/Kyle/Desktop/workspace/vkc/main.cpp > CMakeFiles/vkc.dir/main.cpp.i

CMakeFiles/vkc.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vkc.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/Kyle/Desktop/workspace/vkc/main.cpp -o CMakeFiles/vkc.dir/main.cpp.s

CMakeFiles/vkc.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/vkc.dir/main.cpp.o.requires

CMakeFiles/vkc.dir/main.cpp.o.provides: CMakeFiles/vkc.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/vkc.dir/build.make CMakeFiles/vkc.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/vkc.dir/main.cpp.o.provides

CMakeFiles/vkc.dir/main.cpp.o.provides.build: CMakeFiles/vkc.dir/main.cpp.o


# Object files for target vkc
vkc_OBJECTS = \
"CMakeFiles/vkc.dir/main.cpp.o"

# External object files for target vkc
vkc_EXTERNAL_OBJECTS =

vkc: CMakeFiles/vkc.dir/main.cpp.o
vkc: CMakeFiles/vkc.dir/build.make
vkc: /usr/local/lib/libglfw3.a
vkc: /usr/local/lib/libGLEW.dylib
vkc: /usr/local/lib/libglfw3.a
vkc: /usr/local/lib/libGLEW.dylib
vkc: CMakeFiles/vkc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/Kyle/Desktop/workspace/vkc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable vkc"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vkc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vkc.dir/build: vkc

.PHONY : CMakeFiles/vkc.dir/build

CMakeFiles/vkc.dir/requires: CMakeFiles/vkc.dir/main.cpp.o.requires

.PHONY : CMakeFiles/vkc.dir/requires

CMakeFiles/vkc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vkc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vkc.dir/clean

CMakeFiles/vkc.dir/depend:
	cd /Users/Kyle/Desktop/workspace/vkc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Kyle/Desktop/workspace/vkc /Users/Kyle/Desktop/workspace/vkc /Users/Kyle/Desktop/workspace/vkc/build /Users/Kyle/Desktop/workspace/vkc/build /Users/Kyle/Desktop/workspace/vkc/build/CMakeFiles/vkc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vkc.dir/depend
