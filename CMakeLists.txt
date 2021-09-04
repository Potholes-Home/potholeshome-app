cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Require C++ standard to be supported")

project(pothole_analyzer LANGUAGES CXX C)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(TENSORFLOW_SOURCE_DIR "" CACHE PATH
  "Directory that contains the TensorFlow project" )
if(NOT TENSORFLOW_SOURCE_DIR)
  get_filename_component(TENSORFLOW_SOURCE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/tensorflow"
    ABSOLUTE
  )
endif()

add_subdirectory(
  "${TENSORFLOW_SOURCE_DIR}/tensorflow/lite"
  "${CMAKE_CURRENT_BINARY_DIR}/tensorflow-lite" EXCLUDE_FROM_ALL)


if(NOT MSVC)
	set(
		CMAKE_CXX_FLAGS
		"${CMAKE_CXX_FLAGS} \
		-Wall \
		-Wextra \
		-pipe \
		-fsized-deallocation \
		-fdiagnostics-color=always \
		-Wunreachable-code \
		-Wno-attributes \
                -O3 \
                 -m64"
		)
	if (NOT CMAKE_COMPILER_IS_GNUCXX)
		# -Wshadow is buggy and broken in GCC, so do not enable it.
		# see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=79328
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
	endif()
endif()

if (MSVC)
	set(
		CMAKE_CXX_FLAGS
		"${CMAKE_CXX_FLAGS} /bigobj"
		)
endif()

add_executable(pothole_analyzer
               main.cpp)

target_include_directories(pothole_analyzer PRIVATE
                          ${CMAKE_CURRENT_LIST_DIR}/boinc/api/
                          ${CMAKE_CURRENT_LIST_DIR}/boinc/lib/
                          ${CMAKE_CURRENT_LIST_DIR}/boinc/)

if(NOT MSVC)
target_link_libraries (pothole_analyzer PRIVATE
                      tensorflow-lite
                      -L${CMAKE_CURRENT_LIST_DIR}/boinc/lib
                      -L${CMAKE_CURRENT_LIST_DIR}/boinc/api
                      -L${CMAKE_CURRENT_LIST_DIR}/boinc
                      #-l:boinc_api.a
                      #-l:boinc.a
                      Threads::Threads
                      -lstdc++
                      -static
                      -static-libstdc++
                      -DBOINC
                      -lboinc_api
                      -lboinc)
endif()
if (MSVC)
target_compile_options(pothole_analyzer PRIVATE /MT)
target_link_libraries (pothole_analyzer PRIVATE
                      tensorflow-lite
                      Threads::Threads
                      ${CMAKE_CURRENT_LIST_DIR}/boinc_windows/libboinc.lib
                      ${CMAKE_CURRENT_LIST_DIR}/boinc_windows/libboincapi.lib
                      )
endif()
