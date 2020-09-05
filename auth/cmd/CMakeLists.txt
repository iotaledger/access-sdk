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

set(sources auth_cmd.c)

add_library(auth_cmd ${sources})
add_dependencies(auth_cmd cjson pdp)

set(include
  ${CMAKE_SOURCE_DIR}/policy/pdp/
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${cjson_SOURCE_DIR}
)

message(STATUS testt)

target_include_directories(auth_cmd PUBLIC  )
target_link_directories(auth_cmd PUBLIC ${iota_common_BINARY_DIR}/lib)
target_link_libraries(auth_cmd PRIVATE cjson)

if(TEST)
  add_subdirectory(tests)
endif()
