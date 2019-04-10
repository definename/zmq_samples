project(curve-sub-client)

set(ZMQ_CURVE_CLIENT_SRC
	src/ZmqMonitor.cpp
	src/main.cpp
	)
set(ZMQ_CURVE_CLIENT_HEADERS
	include/ZmqMonitor.h
	)
set(MISC
	src/pch.h
	src/pch.cpp
	)

source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("Header Files" FILES ${PROJECT_HEADERS})
source_group("" FILES ${MISC})

add_definitions(
	-DWIN32_LEAN_AND_MEAN
	-D_CRT_SECURE_NO_WARNINGS
	-DUNICODE
	-D_UNICODE
	)

add_executable(${PROJECT_NAME}
	${ZMQ_CURVE_CLIENT_SRC}
	${ZMQ_CURVE_CLIENT_HEADERS}
	${MISC}
	)

include_directories(
	include
	${ZeroMQ_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
	${libzmqutils_INCLUDE_DIRS}
	)
target_link_libraries(${PROJECT_NAME}
	${ZeroMQ_LIBRARIES}
	${Boost_LIBRARIES}
	${libzmqutils_LIBRARIES}
	)
add_dependencies(${PROJECT_NAME}
	ZeroMQ
	Libzmqutil
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

add_precompiled_headers(${PROJECT_NAME} "src/")

SET_PROPERTY(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${PROPERTY_FOLDER})
#set_target_properties(${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)