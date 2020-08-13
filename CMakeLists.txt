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

# fetch iota_common
include(FetchContent)
FetchContent_Declare(
  iota_common
  GIT_REPOSITORY http://github.com/iotaledger/iota_common.git
  GIT_TAG cf649803757abf48432d4fa60e9f27ac119bae5f
)

# fetch sqlite-amalgamation
include(FetchContent)
FetchContent_Declare(
  sqlite3
  GIT_REPOSITORY https://github.com/azadkuh/sqlite-amalgamation.git
  GIT_TAG 9be05e13408dd8cf8dafe8f11b1700a28194e07e
)

if(auth_implementation STREQUAL openssl)
  # fetch openssl
  include(FetchContent)
  FetchContent_Declare(
    openssl
    GIT_REPOSITORY https://github.com/honeycombOS/openssl-cmake.git
    GIT_TAG 8ffd9276ec8750ae4293cfa0bc9e7562807bb37d
  )
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

message(STATUS "Fetching iota_common")
FetchContent_MakeAvailable(iota_common)

message(STATUS "Fetching sqlite3")
FetchContent_MakeAvailable(sqlite3)

if(auth_implementation STREQUAL openssl)
  message(STATUS "Fetching openssl")
  FetchContent_MakeAvailable(openssl)
endif()

# fetch external libs
include(ExternalProject)

# Global includes
include_directories(utils/misc plugins/data/dataset)

add_subdirectory(auth)
add_subdirectory(policy)
add_subdirectory(plugin)
add_subdirectory(utils)
