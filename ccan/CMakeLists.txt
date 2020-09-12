set(sources
  ccan/base64/base64.c
  ccan/err/err.c
  ccan/str/hex/hex.c
)

set(headers
  config.h
  ccan/base64/base64.h
  ccan/err/err.h
  ccan/str/hex/hex.h
)

set(include
  ${CMAKE_CURRENT_SOURCE_DIR}/
  ccan/err/
  ccan/str/hex/
)

add_library(ccan STATIC ${sources})
target_include_directories(ccan PUBLIC ${include})

install(TARGETS ccan DESTINATION lib)
install(FILES ${headers} DESTINATION include)
