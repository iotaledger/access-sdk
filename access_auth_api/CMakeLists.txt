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

set(target auth)

set(auth_implementation tinyembedded CACHE STRING "auth implementation choice")
#set(auth_implementation openssl CACHE STRING "auth implementation choice")

set(libs ${policy_format} cryptography common)

set(sources
  auth_logger.c
  auth.c
  auth_helper.c
  implementations/${auth_implementation}/auth_client.c
  implementations/${auth_implementation}/auth_server.c
  implementations/${auth_implementation}/auth_client.c
  implementations/${auth_implementation}/auth_utils.c)

set(include_dirs
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/implementations/${auth_implementation})

add_library(${target} ${sources})

target_link_directories(auth PUBLIC
  "${CMAKE_INSTALL_PREFIX}/lib"
  "${iota_common_BUILD_DIR}")

target_include_directories(${target} PUBLIC ${include_dirs})
target_link_libraries(${target} PUBLIC ${libs})
