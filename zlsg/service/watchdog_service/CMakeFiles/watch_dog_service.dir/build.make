# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jwf/code/CPP/zlsg

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jwf/code/CPP/zlsg

# Include any dependencies generated for this target.
include service/watchdog_service/CMakeFiles/watch_dog_service.dir/depend.make

# Include the progress variables for this target.
include service/watchdog_service/CMakeFiles/watch_dog_service.dir/progress.make

# Include the compile flags for this target's objects.
include service/watchdog_service/CMakeFiles/watch_dog_service.dir/flags.make

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o: service/watchdog_service/CMakeFiles/watch_dog_service.dir/flags.make
service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o: service/watchdog_service/watchdog_interface.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jwf/code/CPP/zlsg/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o"
	cd /home/jwf/code/CPP/zlsg/service/watchdog_service && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o -c /home/jwf/code/CPP/zlsg/service/watchdog_service/watchdog_interface.cc

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.i"
	cd /home/jwf/code/CPP/zlsg/service/watchdog_service && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jwf/code/CPP/zlsg/service/watchdog_service/watchdog_interface.cc > CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.i

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.s"
	cd /home/jwf/code/CPP/zlsg/service/watchdog_service && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jwf/code/CPP/zlsg/service/watchdog_service/watchdog_interface.cc -o CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.s

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o.requires:
.PHONY : service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o.requires

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o.provides: service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o.requires
	$(MAKE) -f service/watchdog_service/CMakeFiles/watch_dog_service.dir/build.make service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o.provides.build
.PHONY : service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o.provides

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o.provides.build: service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o: service/watchdog_service/CMakeFiles/watch_dog_service.dir/flags.make
service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o: service/watchdog_service/watchdog_service.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jwf/code/CPP/zlsg/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o"
	cd /home/jwf/code/CPP/zlsg/service/watchdog_service && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o -c /home/jwf/code/CPP/zlsg/service/watchdog_service/watchdog_service.cc

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/watch_dog_service.dir/watchdog_service.cc.i"
	cd /home/jwf/code/CPP/zlsg/service/watchdog_service && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jwf/code/CPP/zlsg/service/watchdog_service/watchdog_service.cc > CMakeFiles/watch_dog_service.dir/watchdog_service.cc.i

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/watch_dog_service.dir/watchdog_service.cc.s"
	cd /home/jwf/code/CPP/zlsg/service/watchdog_service && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jwf/code/CPP/zlsg/service/watchdog_service/watchdog_service.cc -o CMakeFiles/watch_dog_service.dir/watchdog_service.cc.s

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o.requires:
.PHONY : service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o.requires

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o.provides: service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o.requires
	$(MAKE) -f service/watchdog_service/CMakeFiles/watch_dog_service.dir/build.make service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o.provides.build
.PHONY : service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o.provides

service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o.provides.build: service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o

# Object files for target watch_dog_service
watch_dog_service_OBJECTS = \
"CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o" \
"CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o"

# External object files for target watch_dog_service
watch_dog_service_EXTERNAL_OBJECTS =

service/watchdog_service/libwatch_dog_service.a: service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o
service/watchdog_service/libwatch_dog_service.a: service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o
service/watchdog_service/libwatch_dog_service.a: service/watchdog_service/CMakeFiles/watch_dog_service.dir/build.make
service/watchdog_service/libwatch_dog_service.a: service/watchdog_service/CMakeFiles/watch_dog_service.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libwatch_dog_service.a"
	cd /home/jwf/code/CPP/zlsg/service/watchdog_service && $(CMAKE_COMMAND) -P CMakeFiles/watch_dog_service.dir/cmake_clean_target.cmake
	cd /home/jwf/code/CPP/zlsg/service/watchdog_service && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/watch_dog_service.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
service/watchdog_service/CMakeFiles/watch_dog_service.dir/build: service/watchdog_service/libwatch_dog_service.a
.PHONY : service/watchdog_service/CMakeFiles/watch_dog_service.dir/build

service/watchdog_service/CMakeFiles/watch_dog_service.dir/requires: service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_interface.cc.o.requires
service/watchdog_service/CMakeFiles/watch_dog_service.dir/requires: service/watchdog_service/CMakeFiles/watch_dog_service.dir/watchdog_service.cc.o.requires
.PHONY : service/watchdog_service/CMakeFiles/watch_dog_service.dir/requires

service/watchdog_service/CMakeFiles/watch_dog_service.dir/clean:
	cd /home/jwf/code/CPP/zlsg/service/watchdog_service && $(CMAKE_COMMAND) -P CMakeFiles/watch_dog_service.dir/cmake_clean.cmake
.PHONY : service/watchdog_service/CMakeFiles/watch_dog_service.dir/clean

service/watchdog_service/CMakeFiles/watch_dog_service.dir/depend:
	cd /home/jwf/code/CPP/zlsg && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jwf/code/CPP/zlsg /home/jwf/code/CPP/zlsg/service/watchdog_service /home/jwf/code/CPP/zlsg /home/jwf/code/CPP/zlsg/service/watchdog_service /home/jwf/code/CPP/zlsg/service/watchdog_service/CMakeFiles/watch_dog_service.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : service/watchdog_service/CMakeFiles/watch_dog_service.dir/depend

