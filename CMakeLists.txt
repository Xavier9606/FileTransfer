# CMakeList.txt : CMake project for FileTransfer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ClientFileTransfer")

set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Widgets Core Network)

# Add source to this project's executable.
add_executable (ClientFileTransfer "ClientFileTransfer.cpp" "ClientFileTransfer.h" "SocketsAPI.cpp" "SocketsAPI.h" "MyFileAPI.h")

# TODO: Add tests and install targets if needed.

target_link_libraries(ClientFileTransfer Qt5::Core Qt5::Network)
target_link_libraries(ClientFileTransfer ws2_32)

