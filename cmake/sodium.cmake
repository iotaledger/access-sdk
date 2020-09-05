include(ExternalProject)

set(SODIUM_VERSION 1.0.18)
set(SODIUM_TARBALL libsodium-${SODIUM_VERSION}.tar.gz)
set(SODIUM_URL https://download.libsodium.org/libsodium/releases/${SODIUM_TARBALL})
set(SODIUM_SRC ${CMAKE_CURRENT_BINARY_DIR}/src/libsodium-${SODIUM_VERSION})
set(SODIUM_STATIC_LIB ${CMAKE_CURRENT_BINARY_DIR}/lib/libsodium.a)
set(SODIUM_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/include)

file(MAKE_DIRECTORY ${SODIUM_INCLUDES})

ExternalProject_Add(
        sodium_tarball
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}
        SOURCE_DIR ${SODIUM_SRC}
        DOWNLOAD_COMMAND wget ${SODIUM_URL} && tar xvf ${SODIUM_TARBALL} && ${SODIUM_SRC}/autogen.sh
        CONFIGURE_COMMAND ${SODIUM_SRC}/configure --srcdir=${SODIUM_SRC} --prefix=${CMAKE_CURRENT_BINARY_DIR} --enable-static=yes --disable-shared
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        BUILD_BYPRODUCTS ${SODIUM_STATIC_LIB}
)

add_library(sodium STATIC IMPORTED GLOBAL)

add_dependencies(sodium sodium_tarball)

set_target_properties(sodium PROPERTIES IMPORTED_LOCATION ${SODIUM_STATIC_LIB})
set_target_properties(sodium PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SODIUM_INCLUDES})

install(FILES ${SODIUM_STATIC_LIB} LIBRARY DESTINATION lib)
install(FILES ${SODIUM_INCLUDES}/sodium.h DESTINATION lib)