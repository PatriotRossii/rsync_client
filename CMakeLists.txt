project(rsync)
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.71 COMPONENTS program_options REQUIRED)

add_executable(rsync_client
	main.cpp
)

target_link_libraries(rsync_client rsync Boost::program_options)