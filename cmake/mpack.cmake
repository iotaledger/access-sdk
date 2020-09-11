include(ExternalProject)

# fetch mpack
include(FetchContent)
FetchContent_Declare(
  mpack
  GIT_REPOSITORY http://github.com/ludocode/mpack.git
  GIT_TAG e150cb7455cefdab0df6359414e9e4c792723d73 #v1.0
)

message(STATUS "Fetching mpack")
FetchContent_MakeAvailable(mpack)

set(MPACK_SRC ${CMAKE_CURRENT_BINARY_DIR}/_deps/mpack-src/src/mpack)

set(sources
  ${MPACK_SRC}/mpack-common.c
  ${MPACK_SRC}/mpack-expect.c
  ${MPACK_SRC}/mpack-node.c
  ${MPACK_SRC}/mpack-platform.c
  ${MPACK_SRC}/mpack-reader.c
  ${MPACK_SRC}/mpack-writer.c
)

set(headers
  ${MPACK_SRC}/mpack-common.h
  ${MPACK_SRC}/mpack-defaults.h
  ${MPACK_SRC}/mpack-expect.h
  ${MPACK_SRC}/mpack.h
  ${MPACK_SRC}/mpack-node.h
  ${MPACK_SRC}/mpack-platform.h
  ${MPACK_SRC}/mpack-reader.h
  ${MPACK_SRC}/mpack-writer.h
)

add_library(mpack STATIC ${sources})

install(TARGETS mpack DESTINATION lib)
install(FILES ${headers} DESTINATION include)
