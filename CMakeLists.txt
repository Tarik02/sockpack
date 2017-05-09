# Include threads
find_package(Threads)

add_library(sockpack STATIC
	src/sockpack/Endpoint.cpp
	src/sockpack/ServerSocket.cpp
	src/sockpack/Socket.cpp
)

target_include_directories(sockpack
	PRIVATE src
	PUBLIC include)
target_link_libraries(sockpack ${CMAKE_THREAD_LIBS_INIT})
