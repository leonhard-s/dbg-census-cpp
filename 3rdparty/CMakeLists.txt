find_package(GTest GLOBAL)
find_package(httplib REQUIRED GLOBAL)
find_package(OpenSSL GLOBAL)
find_package(ZLIB REQUIRED GLOBAL)

# NOTE (leonhard-s): Some unfortunate verbosity for configuring and building
# ixWebSocket behind the scenes. Needed because a) it's not on vcpkg and b) we
# want it to use the same OpenSSL version.
set(USE_TLS ON CACHE INTERNAL "ixWebSocket configuration" FORCE)
set(USE_OPEN_SSL ON CACHE INTERNAL "ixWebSocket configuration" FORCE)
set(BUILD_SHARED_LIBS OFF)
include(FetchContent)
FetchContent_Declare(ixwebsocket
    GIT_REPOSITORY https://github.com/machinezone/IXWebSocket
    GIT_TAG v11.4.4
    SYSTEM
)
FetchContent_MakeAvailable(ixwebsocket)
