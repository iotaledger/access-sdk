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

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

set(target policy_test)
set(sources policy_test.c)
set(libs policy)

add_executable(policy_test policy_test.c)
add_dependencies(policy_test policy)

target_include_directories(policy_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_directories(policy_test PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(policy_test PUBLIC ${libs})

add_test(policy_test policy_test)
