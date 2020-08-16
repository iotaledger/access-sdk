include(ExternalProject)

set(OPENSSL_VERSION 1.0.2)
set(OPENSSL_TARBALL OpenSSL_1_0_2u.tar.gz)
set(OPENSSL_URL https://github.com/openssl/openssl/archive/${OPENSSL_TARBALL})
set(OPENSSL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/src/openssl-OpenSSL_1_0_2u)
set(OPENSSL_STATIC_SSL_LIB ${CMAKE_CURRENT_BINARY_DIR}/lib/libssl.a)
set(OPENSSL_STATIC_CRYPTO_LIB ${CMAKE_CURRENT_BINARY_DIR}/lib/libcrypto.a)
set(OPENSSL_INCLUDES ${CMAKE_INSTALL_PREFIX}/include)

file(MAKE_DIRECTORY ${OPENSSL_INCLUDES})

ExternalProject_add(
   openssl
   URL ${OPENSSL_URL}
   CONFIGURE_COMMAND ./config --prefix=${CMAKE_CURRENT_BINARY_DIR} --openssldir=${OPENSSL_PREFIX}
   BUILD_IN_SOURCE 1
   BUILD_BYPRODUCTS ${OPENSSL_STATIC_CRYPTO_LIB} ${OPENSSL_STATIC_SSL_LIB}
   INSTALL_COMMAND make install
)

add_library(ssl STATIC IMPORTED GLOBAL)
add_library(crypto STATIC IMPORTED GLOBAL)

add_dependencies(ssl openssl)
add_dependencies(crypto openssl)

set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${OPENSSL_STATIC_SSL_LIB})
set_target_properties(ssl PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INCLUDES})

set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${OPENSSL_STATIC_CRYPTO_LIB})
set_target_properties(crypto PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${OPENSSL_INCLUDES})
