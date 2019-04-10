project(Libzmqutil)

set(PROJECT_HEADERS
	include/App.h
	include/Curve.h
	include/DealerSocket.h
	include/Processor.h
	include/Socket.hpp
	include/SocketMonitor.h
	include/targetver.h
	include/Types.h
	include/PubSocket.h
	include/RepSocket.h
	include/ReqSocket.h
	include/RouterSocket.h
	include/SubSocket.h
	)
set(PROJECT_SOURCES
	src/Curve.cpp
	src/DealerSocket.cpp
	src/Processor.cpp
	src/SocketMonitor.cpp
	src/PubSocket.cpp
	src/RepSocket.cpp
	src/ReqSocket.cpp
	src/RouterSocket.cpp
	src/SubSocket.cpp
	)
set(MISC
	src/pch.h
	src/pch.cpp
	)

add_library(${PROJECT_NAME} STATIC
	${PROJECT_HEADERS}
	${PROJECT_SOURCES}
	${MISC}
	)

source_group("Header Files" FILES ${PROJECT_HEADERS})
source_group("Source Files" FILES ${PROJECT_SOURCES})
source_group("" FILES ${MISC})

target_include_directories(${PROJECT_NAME}
	PUBLIC
		include
	PRIVATE
		${ZeroMQ_INCLUDE_DIRS}
		${MOODYCAMEL_INCLUDE_DIRS}
		)
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		${Logger_LIBRARIES}
		)
add_dependencies(${PROJECT_NAME}
	ZeroMQ
	)
add_definitions(
	-DWIN32_LEAN_AND_MEAN
	-D_CRT_SECURE_NO_WARNINGS
	-DUNICODE
	-D_UNICODE
	)

add_precompiled_headers(${PROJECT_NAME} "src/")
#set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "_com/interface")
#set_target_properties(${PROJECT_NAME} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD 1)