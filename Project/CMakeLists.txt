cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(app LANGUAGES C CXX VERSION 0.0.1)

set(CMAKE_VERBOSE_MAKEFILE FALSE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
 set(CMAKE_C_FLAGS_DEBUG "-g -ggdb -pg -fsanitize=undefined")
 set(CMAKE_C_FLAGS_RELEASE "-O2")
 set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
 set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
endif ()

set(BOOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/boost_1_72_0/")
set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF) 

find_package(facedetect REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/facedetect/"
			 REQUIRED COMPONENTS facedetect)
find_package(Boost REQUIRED COMPONENTS filesystem )

if(WIN32)
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/facedetect/Release/")
else()
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/facedetect/lib/")
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/facedetect/include/")
include_directories( ${BOOST_INCLUDE_DIRS} )
			 
add_executable(app main.cpp)

target_link_libraries( app facedetect::facedetect )
target_link_libraries( app Boost::filesystem Boost::boost )

install(TARGETS app DESTINATION bin)




