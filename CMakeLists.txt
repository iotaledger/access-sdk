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

if(NOT DEFINED TEST)
    option(TEST "Enable tests" ON)
endif()

if(TEST)
    enable_testing()
endif()

# default POLICY_FORMAT
if(NOT DEFINED POLICY_FORMAT)
  set(POLICY_FORMAT json CACHE STRING "Policy Format Choice")
  #set(POLICY_FORMAT protobuf-c CHACE STRING "Policy Format Choice")
endif()

# default AUTH_FLAVOUR
if(NOT DEFINED AUTH_FLAVOUR)
  set(AUTH_FLAVOUR eddsa CACHE STRING "auth flavour choice")
  #set(AUTH_FLAVOUR rsa CACHE STRING "auth flavour choice")
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
if(AUTH_FLAVOUR STREQUAL eddsa)
  include(cmake/libsodium.cmake)
elseif(AUTH_FLAVOUR STREQUAL rsa)
  include(cmake/openssl.cmake)
endif()

# Global includes
include_directories(
  utils/misc
  utils/crypto
  utils/crypto/ref
  plugins/data/dataset
)

add_subdirectory(auth)
add_subdirectory(policy)
add_subdirectory(plugin)
add_subdirectory(utils)
