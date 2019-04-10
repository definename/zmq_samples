project(router_curve_server)

include(PCHSupport)
include(CRTLinkage)

set(PROJECT_SOURCES
	src/main.cpp
	src/ZMQMonitor.cpp
	)
set(PROJECT_HEADERS
	include/ZMQMonitor.h
	)
set(MISC
	src/pch.h
	src/pch.cpp
	)

source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("Header Files" FILES ${PROJECT_HEADERS})
source_group("" FILES ${MISC})

add_executable(${PROJECT_NAME}
	${PROJECT_SOURCES}
	${PROJECT_HEADERS}
	${MISC}
	)

add_definitions(
	-DUNICODE
	-D_UNICODE
	)

add_precompiled_headers(${PROJECT_NAME} "src/")

include_directories(
	include
	${ZeroMQ_INCLUDE_DIR}
	${Boost_INCLUDE_DIRS}
	)
target_link_libraries(${PROJECT_NAME}
	${ZeroMQ_LIBRARIES}
	${Boost_LIBRARIES}
	)
add_dependencies(${PROJECT_NAME}
	ZeroMQ
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

SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER "dealer_router")
#set_target_properties(${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)

