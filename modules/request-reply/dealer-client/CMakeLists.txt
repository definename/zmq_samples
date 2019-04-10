project(dealer-client)

set(PROJECT_HEADERS
	include/ZmqDealer.h
	)
set(PROJECT_SOURCES
	src/ZmqDealer.cpp
	src/main.cpp
	)
set(MISC
	src/pch.h
	src/pch.cpp
	)

source_group("Header Files" FILES ${PROJECT_HEADERS})
source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("" FILES ${MISC})

add_executable(${PROJECT_NAME}
	${PROJECT_SOURCES}
	${PROJECT_HEADERS}
	${MISC}
	)
include_directories(
	include
	${ZeroMQ_INCLUDE_DIRS}
	${MOODYCAMEL_INCLUDE_DIRS}
	)
target_link_libraries(${PROJECT_NAME}
	${ZeroMQ_LIBRARIES}
	${Libzmqutil_LIBRARIES}
	)
add_dependencies(${PROJECT_NAME}
	ZeroMQ
	Libzmqutil
	)
add_definitions(
	-DWIN32_LEAN_AND_MEAN
	-D_CRT_SECURE_NO_WARNINGS
	-DUNICODE
	-D_UNICODE
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
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER ${PROPERTY_FOLDER})
#set_target_properties(${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)