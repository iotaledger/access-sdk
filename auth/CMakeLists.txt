#
# This file is part of the IOTA Access distribution
# (https://github.com/iotaledger/access)
#
# Copyright (c) 2020 IOTA Stiftung
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

add_subdirectory(tcpip)
add_subdirectory(eddsa)
add_subdirectory(cmd)

# libs in the sandbox
link_directories("${CMAKE_INSTALL_PREFIX}/lib")

set(target auth)

set(libs
  base58
  tcpip
  ${policy_format}
  common
  ccan
  eddsa
)

set(sources
  auth.c
  auth_logger.c)

set(include_dirs
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/tcpip
  ${CMAKE_CURRENT_SOURCE_DIR}/eddsa
  ${PROJECT_BINARY_DIR}/include
  ${CMAKE_SOURCE_DIR}/policy/encoding/json/
  ${CMAKE_SOURCE_DIR}/ccan
  ${CMAKE_SOURCE_DIR}/ccan/ccan/base64/
  ${CMAKE_SOURCE_DIR}/ccan/ccan/err/
  ${CMAKE_SOURCE_DIR}/ccan/ccan/str/hex/)

add_library(${target} ${sources})

add_dependencies(${target} ${libs})

target_link_directories(auth PUBLIC
  "${CMAKE_BINARY_DIR}/lib"
  "${CMAKE_BINARY_DIR}/ccan"
  "${CMAKE_INSTALL_PREFIX}/lib"
  "${iota_common_BUILD_DIR}")

target_include_directories(${target} PUBLIC ${include_dirs})
target_link_libraries(${target} PUBLIC ${libs})

install(TARGETS auth DESTINATION lib)
install(FILES auth.h DESTINATION include)

set(AUTH_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(TEST)
  add_subdirectory(tests)
endif()
