/*
 * This file is part of the IOTA Access Distribution
 * (https://github.com/iotaledger/access)
 *
 * Copyright (c) 2020 IOTA Stiftung
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "plugin_manager.h"

#include <stdlib.h>
#include <string.h>

int plugin_manager_init(plugin_manager_t* plugin_manager, size_t plugins_max_num) {
  plugin_manager->plugins = malloc(sizeof(plugin_t) * plugins_max_num);
  plugin_manager->plugins_num = 0;
  plugin_manager->plugins_max_num = plugins_max_num;
  return 0;
}

int plugin_manager_register(plugin_manager_t* pm, plugin_t* plugin) {
  if (pm->plugins_num < pm->plugins_max_num && plugin != NULL) {
    memcpy(&pm->plugins[pm->plugins_num], plugin, sizeof(plugin_t));
    pm->plugins_num++;
    return 0;
  } else {
    return -1;
  }
}

int plugin_manager_get(plugin_manager_t* pm, size_t idx, plugin_t** plugin) {
  if ((pm->plugins_num > 0) && (idx < pm->plugins_num)) {
    *plugin = &pm->plugins[idx];
    return 0;
  } else {
    return -1;
  }
}
