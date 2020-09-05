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

set(target auth_test)
set(sources auth_test.c)
set(libs auth eddsa tcpip pthread)

add_executable(auth_test auth_test.c auth_client_test.c auth_server_test.c)
add_dependencies(auth_test tcpip)

target_include_directories(auth_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${AUTH_SOURCE_DIR}/eddsa)
target_link_directories(auth_test PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(auth_test PUBLIC ${libs})

add_test(auth_test auth_test)
