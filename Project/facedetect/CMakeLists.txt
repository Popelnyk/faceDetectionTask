cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
#################configure_file(facedetect.pc.in facedetect.pc @ONLY)
project(facedetect LANGUAGES C CXX VERSION 0.0.1)

set(CMAKE_VERBOSE_MAKEFILE FALSE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (CMAKE_COMPILER_IS_GNUCXX)
 set(CMAKE_C_FLAGS_DEBUG "-g -ggdb -pg -fsanitize=undefined")
 set(CMAKE_C_FLAGS_RELEASE "-O2")
 set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
 set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_C_FLAGS_RELEASE})
endif ()

set(CMAKE_BUILD_TYPE RELEASE)


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(OpenCV REQUIRED PATHS "${CMAKE_CURRENT_SOURCE_DIR}/opencv/")
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(facedetect SHARED "${CMAKE_CURRENT_SOURCE_DIR}/src/facedetect.cpp")
set_target_properties(facedetect PROPERTIES PUBLIC_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/facedetect.h")
target_include_directories(facedetect PRIVATE include)
target_include_directories(facedetect PRIVATE src)


target_link_libraries( facedetect ${OpenCV_LIBS} )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/deploy.prototxt.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/res10_300x300_ssd_iter_140000.caffemodel DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
message(STATUS "CMAKE_CURRENT_BINARY_DIR = ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "CMAKE_CURRENT_SOURCE_DIR = ${CMAKE_CURRENT_SOURCE_DIR}")

    
message(STATUS "cmake install prefix: ${CMAKE_INSTALL_PREFIX}")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "cmake install prefix: ${CMAKE_INSTALL_PREFIX}")

install(TARGETS
        facedetect
    EXPORT facedetect.pc.in
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
export(TARGETS
        facedetect
    NAMESPACE facedetect::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/facedetectConfig.cmake"
)
install(EXPORT
        facedetect.pc.in
    DESTINATION "${CMAKE_INSTALL_DATADIR}/facedetect/cmake"
    NAMESPACE facedetect::
)


