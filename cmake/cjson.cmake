include(ExternalProject)

set(CJSON_SRC ${CMAKE_CURRENT_BINARY_DIR}/cjson/src/cjson_tarball/)
set(CJSON_STATIC_LIB ${CMAKE_CURRENT_BINARY_DIR}/cjson/src/cjson_tarball-build/libcjson.a)

ExternalProject_Add(
        cjson_tarball
        PREFIX ${PROJECT_BINARY_DIR}/cjson
        DOWNLOAD_DIR ${PROJECT_BINARY_DIR}/download
        DOWNLOAD_NAME cjson_v1.7.12.tar.gz
        URL https://github.com/DaveGamble/cJSON/archive/v1.7.12.tar.gz
        URL_HASH SHA256=760687665ab41a5cff9c40b1053c19572bcdaadef1194e5cba1b5e6f824686e7
        CMAKE_ARGS
        -DENABLE_CJSON_TEST:STRING=Off
        -DENABLE_CJSON_UTILS:STRING=Off
        -DBUILD_SHARED_LIBS:STRING=Off
        -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
        # -DCMAKE_TOOLCHAIN_FILE:STRING=${CMAKE_TOOLCHAIN_FILE}
        # for debug
        # LOG_DOWNLOAD 1
        # LOG_CONFIGURE 1
        # LOG_INSTALL 1
)

add_library(cjson STATIC IMPORTED GLOBAL)

add_dependencies(cjson cjson_tarball)

set_target_properties(cjson PROPERTIES IMPORTED_LOCATION ${CJSON_STATIC_LIB})
set_target_properties(cjson PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CJSON_SRC})

install(FILES ${CJSON_STATIC_LIB} LIBRARY DESTINATION lib)
install(FILES ${CJSON_SRC}/cJSON.h DESTINATION lib)