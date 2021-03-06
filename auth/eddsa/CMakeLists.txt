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

#include(${CMAKE_DIR}/sodium.cmake)

set(sources
  auth_client.c
  auth_server.c
  auth_client.c
  auth_utils.c
  auth_logger.c
  auth_helper.c)

set(include_dirs
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(eddsa STATIC ${sources})
target_include_directories(eddsa PUBLIC ${include_dirs})
target_link_directories(eddsa PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib)
#target_link_libraries(eddsa PRIVATE sodium ${POLICY_FORMAT} tcpip common)
target_link_libraries(eddsa PRIVATE cryptography ${POLICY_FORMAT} tcpip common)

install(TARGETS eddsa DESTINATION lib)