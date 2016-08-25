cmake_minimum_required(VERSION 3.2.1)

project(router_curve_server)

include(PCHSupport)
include(CRTLinkage)

# Source files
set(PROJECT_SRC
	src/main.cpp
	src/ZmqMonitor.cpp
	)
set(PROJECT_HEADERS
	include/ZmqMonitor.h
	)

set(MISC
	src/pch.h
	src/pch.cpp
	)

add_definitions(
	-DWIN32_LEAN_AND_MEAN
	-D_CRT_SECURE_NO_WARNINGS
	-DUNICODE
	-D_UNICODE
	)

add_executable(${PROJECT_NAME}
	${PROJECT_SRC}
	${PROJECT_HEADERS}
	${MISC}
	)

add_precompiled_headers(${PROJECT_NAME} "src/")

# Sets dependencies
include_directories(include
	${ZeroMQ_INCLUDE_DIR}
	)
target_link_libraries(${PROJECT_NAME}
	${ZeroMQ_LIBRARIES}
	)

add_dependencies(${PROJECT_NAME}
	ZeroMQ
	)

# Copies zmq.dll to bin directory
add_custom_command(
	TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
			${ZeroMQ_BINARIES}
			${EXECUTABLE_OUTPUT_PATH}/${CMAKE_CFG_INTDIR}/${ZeroMQ_BINARY_FILENAME}
	COMMENT "Copying ZeroMQ data files..."
	VERBATIM
	)

SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "dealer_router")
#set_target_properties(${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)

