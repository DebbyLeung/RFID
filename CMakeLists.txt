# CMakeList.txt : CMake project for RFID, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("RFID")
find_package(CURL)

# set(CURL C:\\dev\\vcpkg\\packages\\curl_x64-windows)
# Add source to this project's executable.
add_executable (RFID "main.cpp" "serialCom/rfid.cpp" "serialCom/serialCom_v2.cpp")
target_include_directories(RFID PRIVATE include ${CURL_INCLUDE_DIRS})
target_link_libraries(RFID ${CURL_LIBRARIES}) 
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RFID PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
