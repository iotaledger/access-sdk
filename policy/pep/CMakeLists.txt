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

set(target pep)

set(sources pep.c)

set(include_dirs ${CMAKE_CURRENT_SOURCE_DIR})

set(libs
  pdp
  plugin
)

add_library(${target} pep.c)
target_include_directories(${target} PUBLIC ${include_dirs})
target_link_libraries(${target} PUBLIC ${libs})

install(TARGETS pep DESTINATION lib)
install(FILES pep.h DESTINATION include)
install(FILES pep_plugin.h DESTINATION include)
