set(PROJECT_ROOT_DIR "${CMAKE_ARGV3}")
set(PATCH_SOURCE_DIR "${PROJECT_ROOT_DIR}/external/patches/zeromq-4.3.1.patches")

message(STATUS "Patching ZeroMQ library...")

execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${PATCH_SOURCE_DIR}/CMakeLists.txt CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_ROOT_DIR}/cmake/CRTLinkage.cmake cmake/CRTLinkage.cmake)