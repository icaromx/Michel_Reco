# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(michel_reco)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(include)
include_directories(src)
include_directories(apps)

add_library(mymaths src/mymaths.cpp)
add_library(cluster_tools src/cluster_tools.cpp)
add_library(pca src/pca.cpp)

install(TARGETS mymaths DESTINATION lib/)
install(TARGETS cluster_tools DESTINATION lib/)
install(TARGETS pca DESTINATION lib/)

file(COPY setup.sh DESTINATION ${CMAKE_BINARY_DIR})

##############ROOT#############################
# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
#find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist Tree Net)
find_package(ROOT REQUIRED COMPONENTS MathCore Hist Tree)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
add_definitions(${ROOT_CXX_FLAGS})

#---Create executables
add_executable(michel-reco-stopping-mu-selection apps/michel-reco-stopping-mu-selection.cxx)
add_executable(michel-reco-clustering apps/michel-reco-clustering.cxx)

#---Link executable to libraries
target_link_libraries(michel-reco-stopping-mu-selection mymaths)
target_link_libraries(michel-reco-stopping-mu-selection ${ROOT_LIBRARIES})
target_link_libraries(michel-reco-stopping-mu-selection cluster_tools)

target_link_libraries(michel-reco-clustering mymaths)
target_link_libraries(michel-reco-clustering ${ROOT_LIBRARIES})
target_link_libraries(michel-reco-clustering cluster_tools)
target_link_libraries(michel-reco-clustering pca)
