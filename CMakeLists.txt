# CMakeList.txt : CMake project for FileTransfer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ClientFileTransfer" LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Network Core Quick LinguistTools REQUIRED)

set(TS_FILES TestQtProject_en_US.ts)

# Add source to this project's executable.
add_executable (ClientFileTransfer 
  "sources/Main.cpp" "headers/Main.h" 
  "sources/SocketsAPI.cpp" "headers/SocketsAPI.h"
  "headers/MyFileAPI.h" 
  "sources/ChunkInfo.cpp" "headers/ChunkInfo.h"
  "sources/FileInfo.cpp"   "headers/FileInfo.h"
  "headers/FileTransfer.h"
  "sources/FileTransfer.cpp"
  "sources/TransferHandler.cpp" "headers/TransferHandler.h"
  headers/backend.h
  "sources/backend.cpp"
  qml.qrc
  
  ${TS_FILES}
  "sources/TransferHandler.cpp" "headers/TransferHandler.h" 
  "sources/SafeQueue.cpp" "headers/SafeQueue.h")

# TODO: Add tests and install targets if needed.

target_compile_definitions(ClientFileTransfer
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(ClientFileTransfer Qt5::Core Qt5::Network Qt5::Quick)
target_link_libraries(ClientFileTransfer ws2_32)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
