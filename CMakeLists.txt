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

project(access_sdk)

if(NOT DEFINED policy_format)
  set(policy_format json CACHE STRING "Policy Format Choice")
  #set(policy_format protobuf-c CHACE STRING "Policy Format Choice")
endif()

if(NOT DEFINED auth_implementation)
  set(auth_implementation openssl CACHE STRING "auth implementation choice")
  #set(auth_implementation tinyauth CHACE STRING "auth implementation choice")
endif()

if(NOT DEFINED TEST)
    option(TEST "Enable tests" ON)
endif()

if(TEST)
    enable_testing()
endif()

if(${CMAKE_VERSION} VERSION_LESS 3.14)
    macro(FetchContent_MakeAvailable NAME)
        FetchContent_GetProperties(${NAME})
        if(NOT ${NAME}_POPULATED)
            FetchContent_Populate(${NAME})
            add_subdirectory(${${NAME}_SOURCE_DIR} ${${NAME}_BINARY_DIR})
        endif()
    endmacro()
endif()

include(cmake/iota_common.cmake)
include(cmake/sqlite3.cmake)
include(cmake/libsodium.cmake)
if(auth_flavour STREQUAL openssl)
  include(cmake/openssl.cmake)
endif()

# Global includes
include_directories(utils/misc plugins/data/dataset)

add_subdirectory(auth)
add_subdirectory(policy)
add_subdirectory(plugin)
add_subdirectory(utils)
