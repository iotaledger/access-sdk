
include(ExternalProject)

set(SODIUM_VERSION 1.0.18)
set(SODIUM_TARBALL libsodium-${SODIUM_VERSION}.tar.gz)
set(SODIUM_URL https://download.libsodium.org/libsodium/releases/${SODIUM_TARBALL})
set(SODIUM_SRC ${CMAKE_CURRENT_BINARY_DIR}/src/libsodium-${SODIUM_VERSION})
set(SODIUM_STATIC_LIB ${CMAKE_CURRENT_BINARY_DIR}/lib/libsodium.a)
set(SODIUM_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/include)

file(MAKE_DIRECTORY ${SODIUM_INCLUDES})

ExternalProject_Add(
    sodium
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    SOURCE_DIR ${SODIUM_SRC}
    DOWNLOAD_COMMAND wget ${SODIUM_URL} && tar xvf ${SODIUM_TARBALL} && ${SODIUM_SRC}/autogen.sh
    CONFIGURE_COMMAND ${SODIUM_SRC}/configure --srcdir=${SODIUM_SRC} --prefix=${CMAKE_CURRENT_BINARY_DIR} --enable-static=yes --disable-shared --host=${CMAKE_SYSTEM_PROCESSOR}
    BUILD_COMMAND make
    INSTALL_COMMAND make install
    BUILD_BYPRODUCTS ${SODIUM_STATIC_LIB}
)

add_library(libsodium STATIC IMPORTED GLOBAL)

