include(ExternalProject)

set(BASE58_VERSION 0.1.4)
set(BASE58_TARBALL v${BASE58_VERSION}.tar.gz)
set(BASE58_URL https://github.com/bitcoin/libbase58/archive/${BASE58_TARBALL})
set(BASE58_SRC ${CMAKE_CURRENT_BINARY_DIR}/src/libbase58-${BASE58_VERSION})
set(BASE58_STATIC_LIB ${CMAKE_CURRENT_BINARY_DIR}/lib/libbase58.a)
set(BASE58_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/include)

file(MAKE_DIRECTORY ${BASE58_INCLUDES})

ExternalProject_Add(
        base58_tarball
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}
        SOURCE_DIR ${BASE58_SRC}
        DOWNLOAD_COMMAND wget ${BASE58_URL} && tar xvf ${BASE58_TARBALL} && ${BASE58_SRC}/autogen.sh
        CONFIGURE_COMMAND ${BASE58_SRC}/configure --srcdir=${BASE58_SRC} --prefix=${CMAKE_CURRENT_BINARY_DIR} --enable-static=yes --disable-shared
        BUILD_COMMAND make
        INSTALL_COMMAND make install
        BUILD_BYPRODUCTS ${BASE58_STATIC_LIB}
)

add_library(base58 STATIC IMPORTED GLOBAL)

add_dependencies(base58 base58_tarball)

set_target_properties(base58 PROPERTIES IMPORTED_LOCATION ${BASE58_STATIC_LIB})
set_target_properties(base58 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${BASE58_INCLUDES})

install(FILES ${BASE58_STATIC_LIB} LIBRARY DESTINATION lib)
install(FILES ${BASE58_INCLUDES}/libbase58.h DESTINATION lib)
