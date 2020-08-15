# fetch openssl
include(FetchContent)
FetchContent_Declare(
  openssl
  GIT_REPOSITORY https://github.com/honeycombOS/openssl-cmake.git
  GIT_TAG 8ffd9276ec8750ae4293cfa0bc9e7562807bb37d
)

message(STATUS "Fetching openssl")
FetchContent_MakeAvailable(openssl)
