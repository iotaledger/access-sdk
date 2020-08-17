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

cmake_minimum_required(VERSION 3.11)

# libs in the sandbox
link_directories("${CMAKE_INSTALL_PREFIX}/lib")

add_subdirectory(tcpip)
add_subdirectory(${AUTH_FLAVOUR})

set(include_dirs
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../policy/encoding/${POLICY_FORMAT}
  ${iota_common_SOURCE_DIR})

# libauth.a
add_library(auth STATIC auth.c)
target_link_directories(auth PUBLIC
  ${iota_common_BUILD_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}/${AUTH_FLAVOUR})
target_include_directories(auth PUBLIC ${include_dirs})
target_link_libraries(auth PRIVATE ${AUTH_FLAVOUR} ${POLICY_FORMAT})

install(TARGETS auth DESTINATION lib)
install(FILES auth.h DESTINATION include)

set(AUTH_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(TEST)
  add_subdirectory(tests)
endif()
