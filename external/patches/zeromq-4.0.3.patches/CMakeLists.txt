#------------------------------------------------------------------------------
#
#  Copyright (C) 2009-2011  Artem Rodygin
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#------------------------------------------------------------------------------

# Hint ########################################################################
#	Specify Sodium includes/library path:
#	${SODIUM_INCLUDE_DIRS}
#	${SODIUM_LIBRARIES}
###############################################################################

project(zmq)
cmake_minimum_required(VERSION 2.6)

include(${CMAKE_SOURCE_DIR}/cmake/CRTLinkage.cmake)

option (WITH_LIBSODIUM "Use libsodium for CURVE security" OFF)
if (WITH_LIBSODIUM)
		message (STATUS "Using libsodium for CURVE security")
		if (${SODIUM_INCLUDE_DIRS} STREQUAL "")
			message (FATAL_ERROR "Unable to locate Sodium includes.")
		endif()
		message (STATUS "Sodium includes: ${SODIUM_INCLUDE_DIRS}")
		
		if (${SODIUM_LIBRARIES} STREQUAL "")
			message (FATAL_ERROR "Unable to locate Sodium library.")
		endif()
		message (STATUS "Sodium libraries: ${SODIUM_LIBRARIES}")
		
        include_directories (${SODIUM_INCLUDE_DIRS})
		 # In order to link statically define SODIUM_STATIC
		add_definitions(-DSODIUM_STATIC -DHAVE_LIBSODIUM)
endif (WITH_LIBSODIUM)

if (MSVC)

    configure_file(${CMAKE_SOURCE_DIR}/builds/msvc/platform.hpp ${CMAKE_SOURCE_DIR}/src/platform.hpp COPYONLY)

    add_definitions(-W0)

    add_definitions(-DDLL_EXPORT)
    add_definitions(-DFD_SETSIZE=2048)

else (MSVC)

    set(HEADER_WARNING "Do not edit. Generated from \"zeromq/hdr/platform.hpp.cmake\" by cmake.")

    include(CheckIncludeFile)
    include(CheckFunctionExists)
    include(CheckLibraryExists)
    include(CheckSymbolExists)
    include(CheckCXXSourceCompiles)

    CHECK_INCLUDE_FILE(alloca.h         HAVE_ALLOCA_H)
    CHECK_INCLUDE_FILE(arpa/inet.h      HAVE_ARPA_INET_H)
    CHECK_INCLUDE_FILE(dlfcn.h          HAVE_DLFCN_H)
    CHECK_INCLUDE_FILE(errno.h          HAVE_ERRNO_H)
    CHECK_INCLUDE_FILE(ifaddrs.h        HAVE_IFADDRS_H)
    CHECK_INCLUDE_FILE(inttypes.h       HAVE_INTTYPES_H)
    CHECK_INCLUDE_FILE(limits.h         HAVE_LIMITS_H)
    CHECK_INCLUDE_FILE(memory.h         HAVE_MEMORY_H)
    CHECK_INCLUDE_FILE(netinet/in.h     HAVE_NETINET_IN_H)
    CHECK_INCLUDE_FILE(netinet/tcp.h    HAVE_NETINET_TCP_H)
    CHECK_INCLUDE_FILE(stdbool.h        HAVE_STDBOOL_H)
    CHECK_INCLUDE_FILE(stddef.h         HAVE_STDDEF_H)
    CHECK_INCLUDE_FILE(stdint.h         HAVE_STDINT_H)
    CHECK_INCLUDE_FILE(stdlib.h         HAVE_STDLIB_H)
    CHECK_INCLUDE_FILE(string.h         HAVE_STRING_H)
    CHECK_INCLUDE_FILE(strings.h        HAVE_STRINGS_H)
    CHECK_INCLUDE_FILE(sys/eventfd.h    HAVE_SYS_EVENTFD_H)
    CHECK_INCLUDE_FILE(sys/socket.h     HAVE_SYS_SOCKET_H)
    CHECK_INCLUDE_FILE(sys/stat.h       HAVE_SYS_STAT_H)
    CHECK_INCLUDE_FILE(sys/time.h       HAVE_SYS_TIME_H)
    CHECK_INCLUDE_FILE(sys/types.h      HAVE_SYS_TYPES_H)
    CHECK_INCLUDE_FILE(time.h           HAVE_TIME_H)
    CHECK_INCLUDE_FILE(unistd.h         HAVE_UNISTD_H)
    CHECK_INCLUDE_FILE(windows.h        HAVE_WINDOWS_H)

    if (HAVE_STDDEF_H AND HAVE_STDINT_H)
    set(STDC_HEADERS TRUE)
    endif (HAVE_STDDEF_H AND HAVE_STDINT_H)

    if (HAVE_SYS_TIME_H AND HAVE_TIME_H)
    set(TIME_WITH_SYS_TIME TRUE)
    endif (HAVE_SYS_TIME_H AND HAVE_TIME_H)

    if (HAVE_SYS_EVENTFD_H)
    set(ZMQ_HAVE_EVENTFD TRUE)
    endif (HAVE_SYS_EVENTFD_H)

    CHECK_FUNCTION_EXISTS(freeifaddrs   HAVE_FREEIFADDRS)
    CHECK_FUNCTION_EXISTS(getifaddrs    HAVE_GETIFADDRS)
    CHECK_FUNCTION_EXISTS(gettimeofday  HAVE_GETTIMEOFDAY)
    CHECK_FUNCTION_EXISTS(memset        HAVE_MEMSET)
    CHECK_FUNCTION_EXISTS(perror        HAVE_PERROR)
    CHECK_FUNCTION_EXISTS(socket        HAVE_SOCKET)

    CHECK_LIBRARY_EXISTS("pthread" "pthread_create" "" HAVE_LIBPTHREAD)
    CHECK_LIBRARY_EXISTS("stdc++"  "malloc"         "" HAVE_LIBSTDC__)
    CHECK_LIBRARY_EXISTS("m"       "matherr"        "" HAVE_LIBM)

    CHECK_SYMBOL_EXISTS("_Bool" stdbool.h HAVE__BOOL)

    add_definitions(-D_REENTRANT)
    add_definitions(-D_PTHREAD_SAFE)

    string(TOLOWER ${CMAKE_HOST_SYSTEM_NAME} HOST_OS)

    CHECK_INCLUDE_FILE(sys/eventfd.h    ZMQ_HAVE_EVENTFD)
    CHECK_INCLUDE_FILE(ifaddrs.h        ZMQ_HAVE_IFADDRS)

    if (WIN32)
    set(LT_OBJDIR "_libs/")
    else (WIN32)
    set(LT_OBJDIR ".libs/")
    endif (WIN32)

    set(RETSIGTYPE_SRC "
        #include <sys/types.h>
        #include <signal.h>
        int main ()
        {
            return *(signal (0, 0)) (0) == 1;
            return 0;
        }")

    CHECK_CXX_SOURCE_COMPILES("${RETSIGTYPE_SRC}" RETSIGTYPE_RES)

    if (RETSIGTYPE_RES)
    set(RETSIGTYPE "int")
    else (RETSIGTYPE_RES)
    set(RETSIGTYPE "void")
    endif (RETSIGTYPE_RES)

    configure_file(src/platform.hpp.cmake ${CMAKE_BINARY_DIR}/build/include/zeromq/platform.hpp)

endif (MSVC)

aux_source_directory(src ${PROJECT_NAME}_SRC)

include_directories(${CMAKE_BINARY_DIR}/include
                    ${CMAKE_BINARY_DIR}/build/include/zeromq
                    ${PROJECT_SOURCE_DIR})

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC})
if (WITH_LIBSODIUM)
	target_link_libraries (${PROJECT_NAME} ${SODIUM_LIBRARIES})
endif (WITH_LIBSODIUM)

install(
    DIRECTORY include
    DESTINATION .
    PATTERN ".svn" EXCLUDE
)

install( 
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib)

if (WIN32)
target_link_libraries(${PROJECT_NAME} ws2_32 rpcrt4)
endif (WIN32)

if (HAVE_LIBUUID)
target_link_libraries(${PROJECT_NAME} uuid)
endif (HAVE_LIBUUID)

message(STATUS "Target '${PROJECT_NAME}' is configured")
message("---------------------------------------------")