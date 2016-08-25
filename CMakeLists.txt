cmake_minimum_required(VERSION 3.2.1)

project("zmq_samples")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
option(BUILD_WITH_STATIC_CRT "Use static run-time libraries (/MT or /MTd linker flags)" OFF)
option(BUILD_SHARED_LIBS "Build shared libraries (.dll) instead of static ones (.lib)" OFF)

# Sets executable output folder
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include(ExternalProject)

#External directory path
set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/external)
#Patch directory path
set(PATCH_PATH ${EXTERNAL_PATH}/patches)
#External install directory path
set(EXTERNAL_INSTALL_PATH ${CMAKE_BINARY_DIR}/external)
#ZeroMQ root directory
set(ZEROMQ_ROOT ${EXTERNAL_INSTALL_PATH}/ZeroMQ)
#Libsodium root directory
set(LIBSODIUM_ROOT ${EXTERNAL_INSTALL_PATH}/Libsodium)

#Add Libsodium as external library
ExternalProject_Add(
	Libsodium
	URL "${EXTERNAL_PATH}/libsodium-1.0.8.zip"
	URL_MD5 F520464587095D3FF6556EC526C83677
	CMAKE_ARGS
		-DBUILD_WITH_STATIC_CRT:BOOL=${BUILD_WITH_STATIC_CRT}
		-DLIBSODIUM_BUILD_SHARED_LIBRARIES:BOOL=OFF
		-DCMAKE_INSTALL_PREFIX:PATH=${LIBSODIUM_ROOT}
	PATCH_COMMAND
		${CMAKE_COMMAND} -P ${PATCH_PATH}/libsodium-1.0.8.patches/patch.cmake ${CMAKE_SOURCE_DIR}
	)
set(SODIUM_INCLUDE_DIRS ${LIBSODIUM_ROOT}/include)
set(SODIUM_LIBRARIES ${LIBSODIUM_ROOT}/lib/static/libsodium.lib)

#Add ZeroMQ as external library
#ExternalProject_Add(
#	ZeroMQ
#	DEPENDS
#		Libsodium
#	URL "${EXTERNAL_PATH}/zeromq-4.2.0.zip"
#	URL_MD5 885C29CF0C0A9D1FFC88CE02C9E7FA61
#	CMAKE_ARGS
#		-DBUILD_WITH_STATIC_CRT:BOOL=${BUILD_WITH_STATIC_CRT}
#		-DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
#		-DCMAKE_INSTALL_PREFIX:PATH=${ZEROMQ_ROOT}
#		-DSODIUM_INCLUDE_DIRS:PATH=${SODIUM_INCLUDE_DIRS}
#		-DSODIUM_LIBRARIES:PATH=${SODIUM_LIBRARIES}
#		-DWITH_LIBSODIUM:BOOL=ON
#		-DZMQ_BUILD_TESTS:BOOL=OFF
#	PATCH_COMMAND
#		${CMAKE_COMMAND} -P ${PATCH_PATH}/zeromq-4.2.0.patches/patch.cmake ${CMAKE_SOURCE_DIR}
#	)
ExternalProject_Add(
	ZeroMQ
	DEPENDS 
#		zlib
		Libsodium
	URL "${EXTERNAL_PATH}/zeromq-4.0.3.zip"
	URL_MD5 6B6C72679E0EF06F10B5F18FE8543C20
	CMAKE_ARGS
		-DBUILD_WITH_STATIC_CRT:BOOL=${BUILD_WITH_STATIC_CRT}
		-DCMAKE_INSTALL_PREFIX:PATH=${ZEROMQ_ROOT}
		-DWITH_LIBSODIUM:BOOL=ON
		-DSODIUM_INCLUDE_DIRS:PATH=${SODIUM_INCLUDE_DIRS}
		-DSODIUM_LIBRARIES:PATH=${SODIUM_LIBRARIES}
	PATCH_COMMAND
		${CMAKE_COMMAND} -P ${PATCH_PATH}/zeromq-4.0.3.patches/patch.cmake ${CMAKE_SOURCE_DIR}
		)
set(ZeroMQ_INCLUDE_DIR ${ZEROMQ_ROOT}/include)
set(ZeroMQ_LIBRARIES ${ZEROMQ_ROOT}/lib/zmq.lib)
set(ZeroMQ_BINARY_FILENAME zmq.dll)
set(ZeroMQ_BINARIES ${ZEROMQ_ROOT}/bin/${ZeroMQ_BINARY_FILENAME})

# Use folder in project
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
SET_PROPERTY(TARGET ZeroMQ PROPERTY FOLDER "External")
SET_PROPERTY(TARGET Libsodium PROPERTY FOLDER "External")

add_subdirectory(modules/zmq_curve_client)
add_subdirectory(modules/zmq_curve_server)
add_subdirectory(modules/zmq_monitor_client)
add_subdirectory(modules/zmq_monitor_server)

add_subdirectory(modules/req_rep/req_curve_client)
add_subdirectory(modules/req_rep/rep_curve_server)

add_subdirectory(modules/dealer_router/dealer_curve_client)
add_subdirectory(modules/dealer_router/router_curve_server)


add_subdirectory(modules/zmq_poll/sender)
add_subdirectory(modules/zmq_poll/receiver)
add_subdirectory(modules/zmq_version)