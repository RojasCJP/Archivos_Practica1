# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/163/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/163/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Archivos_Practica1.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Archivos_Practica1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Archivos_Practica1.dir/flags.make

CMakeFiles/Archivos_Practica1.dir/main.cpp.o: CMakeFiles/Archivos_Practica1.dir/flags.make
CMakeFiles/Archivos_Practica1.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Archivos_Practica1.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Archivos_Practica1.dir/main.cpp.o -c /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/main.cpp

CMakeFiles/Archivos_Practica1.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Archivos_Practica1.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/main.cpp > CMakeFiles/Archivos_Practica1.dir/main.cpp.i

CMakeFiles/Archivos_Practica1.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Archivos_Practica1.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/main.cpp -o CMakeFiles/Archivos_Practica1.dir/main.cpp.s

# Object files for target Archivos_Practica1
Archivos_Practica1_OBJECTS = \
"CMakeFiles/Archivos_Practica1.dir/main.cpp.o"

# External object files for target Archivos_Practica1
Archivos_Practica1_EXTERNAL_OBJECTS =

Archivos_Practica1: CMakeFiles/Archivos_Practica1.dir/main.cpp.o
Archivos_Practica1: CMakeFiles/Archivos_Practica1.dir/build.make
Archivos_Practica1: CMakeFiles/Archivos_Practica1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Archivos_Practica1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Archivos_Practica1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Archivos_Practica1.dir/build: Archivos_Practica1
.PHONY : CMakeFiles/Archivos_Practica1.dir/build

CMakeFiles/Archivos_Practica1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Archivos_Practica1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Archivos_Practica1.dir/clean

CMakeFiles/Archivos_Practica1.dir/depend:
	cd /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1 /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1 /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/cmake-build-debug /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/cmake-build-debug /home/juanpa/Documents/Archivos/Laboratorio/Archivos_Practica1/cmake-build-debug/CMakeFiles/Archivos_Practica1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Archivos_Practica1.dir/depend

