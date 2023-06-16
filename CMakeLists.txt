cmake_minimum_required(VERSION 3.25)
project(cppproj1)

set(CMAKE_CXX_STANDARD 20)

find_package(CURL CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)

add_executable(main main.cpp GUI/App.cpp GUI/MyFrame.cpp core/WoWAPI.cpp core/AuctionEntry.cpp core/AuctionHouse.cpp)

target_link_libraries(main PRIVATE CURL::libcurl)
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(main PRIVATE wx::core wx::base)