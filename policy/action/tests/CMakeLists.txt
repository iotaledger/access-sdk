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

set(target action_test)
set(sources action_test.c)
set(libs action)

add_executable(action_test action_test.c)
add_dependencies(action_test action)

target_include_directories(action_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_directories(action_test PUBLIC ${CMAKE_BINARY_DIR}/lib)
target_link_libraries(action_test PUBLIC ${libs})

add_test(action_test action_test)
