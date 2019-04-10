project(qt-broker-client)

set(PROJECT_SOURCES
	src/ZMQMonitor.cpp
	src/main.cpp
	)
set(PROJECT_HEADERS
	include/ZMQMonitor.h
	)
set(MISC
	src/pch.h
	src/pch.cpp
	)

set(PROTOFILE_PATH ${CMAKE_CURRENT_LIST_DIR}/protofiles)
set(PROTOFILES
	${PROTOFILE_PATH}/MessageBroker.proto
	)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOFILES})
set(PROTOBUF_SOURCES
	${PROTO_SRCS}
	${PROTO_HDRS}
	)

source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("Header Files" FILES ${PROJECT_HEADERS})
source_group("Protobuf" FILES ${PROTOBUF_SOURCES})
source_group("" FILES ${MISC})

set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS} PROPERTIES COMPILE_FLAGS "/Y-")

add_executable(${PROJECT_NAME}
	${PROJECT_SOURCES}
	${PROJECT_HEADERS}
	${MISC}
	${PROTOBUF_SOURCES}
	)

add_definitions(
	-DWIN32_LEAN_AND_MEAN
	-D_CRT_SECURE_NO_WARNINGS
	-DUNICODE
	-D_UNICODE
	)

add_precompiled_headers(${PROJECT_NAME} "src/")

include_directories(
	include
	${CMAKE_CURRENT_BINARY_DIR}
	${ZeroMQ_INCLUDE_DIR}
	${ZLIB_INCLUDE_DIR}
	${Protobuf_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${libzmqutils_INCLUDE_DIRS}
	)
target_link_libraries(${PROJECT_NAME}
	${ZeroMQ_LIBRARIES}
	${ZLIB_LIBRARY}
	${Protobuf_LIBRARIES}
	${Boost_LIBRARIES}
	${libzmqutils_LIBRARIES}
	)

add_dependencies(${PROJECT_NAME}
	ZeroMQ
	zlib
	protobuf
	libzmqutils
	)

add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${ZeroMQ_BINARIES}
			${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/${ZeroMQ_BINARY_FILENAME}
	COMMENT "Copying ZeroMQ data files..."
	VERBATIM
	)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${PROPERTY_FOLDER})
#set_target_properties(${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)

