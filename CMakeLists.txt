cmake_minimum_required(VERSION 3.13.1)

project(zmq_samples)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

###########################################################
# Global includes #########################################

include(ExternalProject)
include(PCHSupport)
include(CRTLinkage)

###########################################################
# Build options ###########################################

option(BUILD_WITH_BOOST "Build project with boost libraries" ON)
option(BUILD_WITH_LIBSODIUM "Build project with libsodium support" OFF)
option(BUILD_WITH_ZLIB "Build project with zlib support" OFF)
option(BUILD_WITH_PROTOBUF "Build project with protobuf support" OFF)
option(BUILD_WITH_ZMQ "Build project with zmq support" ON)

# Use the latest version of CPP stantard.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
# Sets executable output folder
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
# Modules directory.
set(MODULES_PATH ${CMAKE_SOURCE_DIR}/modules)
# External directory.
set(EXTERNAL_DIRECTORY ${CMAKE_SOURCE_DIR}/external)
# Patch directory.
set(PATCH_PATH ${EXTERNAL_DIRECTORY}/patches)
# External install directory.
set(EXTERNAL_INSTALL_PATH ${CMAKE_BINARY_DIR}/external)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

###########################################################
# Boost ###################################################

if(BUILD_WITH_BOOST)
	set(Boost_USE_STATIC_LIBS ON)
	set(Boost_USE_MULTITHREADED ON)
	set(Boost_USE_STATIC_RUNTIME ON)
	find_package(Boost REQUIRED COMPONENTS
		filesystem
		thread
		)
endif(BUILD_WITH_BOOST)

###########################################################
# LIBSODIUM ###############################################

if(BUILD_WITH_LIBSODIUM)
	set(LIBSODIUM_ROOT ${EXTERNAL_INSTALL_PATH}/Libsodium)
	ExternalProject_Add(
		Libsodium
		URL "${EXTERNAL_DIRECTORY}/libsodium-1.0.8.zip"
		URL_MD5 F520464587095D3FF6556EC526C83677
		CMAKE_ARGS
			-DBUILD_WITH_STATIC_CRT:BOOL=ON
			-DLIBSODIUM_BUILD_SHARED_LIBRARIES:BOOL=OFF
			-DCMAKE_INSTALL_PREFIX:PATH=${LIBSODIUM_ROOT}
		PATCH_COMMAND
			${CMAKE_COMMAND} -P ${PATCH_PATH}/libsodium-1.0.8.patches/patch.cmake ${CMAKE_SOURCE_DIR}
		)
	set(SODIUM_INCLUDE_DIRS ${LIBSODIUM_ROOT}/include)
	set(SODIUM_LIBRARIES ${LIBSODIUM_ROOT}/lib/static/libsodium.lib)

	SET_PROPERTY(TARGET Libsodium PROPERTY FOLDER "External")
endif()

###########################################################
# ZLIB ####################################################

if(BUILD_WITH_ZLIB)	
	set(ZLIB_ROOT ${EXTERNAL_INSTALL_PATH}/ZLIB)
	ExternalProject_Add(
		zlib
		URL "${EXTERNAL_DIRECTORY}/zlib-1.2.8.zip"
		URL_MD5 D34BAA0DA16E33A5EF095E7EEF7D2472
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=${ZLIB_ROOT}
			-DBUILD_SHARED_LIBS:BOOL=OFF
		PATCH_COMMAND 
			${CMAKE_COMMAND} -P ${PATCH_PATH}/zlib-1.2.8.patches/patch.cmake ${CMAKE_SOURCE_DIR}
		)

	set(ZLIB_INCLUDE_DIR ${ZLIB_ROOT}/include)
	set(ZLIB_LIBRARY
		debug ${ZLIB_ROOT}/lib/zlibstaticd.lib
		optimized ${ZLIB_ROOT}/lib/zlibstatic.lib
		)

	SET_PROPERTY(TARGET zlib PROPERTY FOLDER "External")
endif(BUILD_WITH_ZLIB)

############################################################
# PROTOBUF #################################################

if(BUILD_WITH_PROTOBUF)
	set(PROTOBUF_ROOT ${EXTERNAL_INSTALL_PATH}/PROTOBUF)
	ExternalProject_Add(
		protobuf
		DEPENDS
			zlib
		URL "${EXTERNAL_DIRECTORY}/protobuf-cpp-3.1.0.zip"
		URL_MD5 3325958B606A6AF351E2BFBF4250EC1C
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=${PROTOBUF_ROOT}
			-DBUILD_SHARED_LIBS:BOOL=OFF
			-DZLIB_ROOT:PATH=${ZLIB_ROOT}
		SOURCE_SUBDIR cmake
		PATCH_COMMAND 
			${CMAKE_COMMAND} -P ${PATCH_PATH}/protobuf-cpp-3.1.0.patches/patch.cmake ${CMAKE_SOURCE_DIR}
		)

	set(Protobuf_INCLUDE_DIR ${PROTOBUF_ROOT}/include)
	set(Protobuf_PROTOC_EXECUTABLE ${PROTOBUF_ROOT}/bin/protoc.exe)
	set(Protobuf_LIBRARY
		debug ${PROTOBUF_ROOT}/lib/libprotobufd.lib
		optimized ${PROTOBUF_ROOT}/lib/libprotobuf.lib)
	set(Protobuf_PROTOC_LIBRARY 
		debug ${PROTOBUF_ROOT}/lib/libprotocd.lib
		optimized ${PROTOBUF_ROOT}/lib/libprotoc.lib)
	set(Protobuf_LITE_LIBRARY
		debug ${PROTOBUF_ROOT}/lib/libprotobuf-lited.lib
		optimized ${PROTOBUF_ROOT}/lib/libprotobuf-lite.lib)

	find_package(protobuf REQUIRED)

	SET_PROPERTY(TARGET protobuf PROPERTY FOLDER "External")
endif()


##########################################################
# ZEROMQ #################################################

if(BUILD_WITH_ZMQ)
	set(ZEROMQ_ROOT ${EXTERNAL_INSTALL_PATH}/ZeroMQ)
	ExternalProject_Add(
		ZeroMQ
		URL "${EXTERNAL_DIRECTORY}/zeromq-4.3.1.zip"
		URL_MD5 903290b6272530e6a38f72288312f919
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX:PATH=${ZEROMQ_ROOT}
			-DWITH_LIBSODIUM:BOOL=OFF
			-DBUILD_STATIC:BOOL=OFF
			-DBUILD_TESTS:BOOL=OFF
			-DBUILD_SHARED:BOOL=ON
			-DBUILD_WITH_STATIC_CRT:BOOL=ON
		PATCH_COMMAND
			${CMAKE_COMMAND} -P ${PATCH_PATH}/zeromq-4.3.1.patches/patch.cmake ${CMAKE_SOURCE_DIR}
			)
	set(ZeroMQ_INCLUDE_DIRS ${ZEROMQ_ROOT}/include)
	set(ZeroMQ_LIBRARIES ${ZEROMQ_ROOT}/lib/libzmq.lib)
	set(ZeroMQ_BINARY_FILENAME libzmq.dll)
	set(ZeroMQ_BINARIES ${ZEROMQ_ROOT}/bin/${ZeroMQ_BINARY_FILENAME})

	SET_PROPERTY(TARGET ZeroMQ PROPERTY FOLDER "External")
endif()

###########################################################
# spdlog ##################################################

set(SPDLOG_INCLUDE_DIRS ${EXTERNAL_DIRECTORY}/spdlog/include)

###########################################################
# Logger ##################################################

set(Logger_LIBRARIES Logger)
add_subdirectory(modules/logger)

###########################################################
# moodycamel ##############################################

set(MOODYCAMEL_INCLUDE_DIRS ${EXTERNAL_DIRECTORY}/moodycamel/include)

###########################################################
# Libzmqutil ##############################################

# set(Libzmqutil_LIBRARIES Libzmqutil)
# add_subdirectory(modules/libzmqutils)


add_subdirectory(modules/broker-client)
# add_subdirectory(modules/publish-subscribe)
# add_subdirectory(modules/request-reply)


#add_subdirectory(modules/dealer_router/dealer_curve_client)
#add_subdirectory(modules/dealer_router/router_curve_server)


#add_subdirectory(modules/zmq_poll/sender)
#add_subdirectory(modules/zmq_poll/receiver)
#add_subdirectory(modules/zmq_version)