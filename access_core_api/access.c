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

#include "access.h"

#include <stdlib.h>
#include <string.h>

#include "pap_plugin.h"
#include "pep.h"
#include "pep_plugin.h"
#include "pip.h"
#include "timer.h"

// currently empty
// placeholder context structure for future use
typedef struct {
} access_ctx_t_;

void access_init(access_ctx_t *access_context) {
  access_ctx_t_ *ctx = calloc(1, sizeof(access_ctx_t_));

  pep_init();

  *access_context = (access_ctx_t)ctx;
}

void access_start(access_ctx_t access_context) {
  access_ctx_t_ *ctx = (access_ctx_t_ *)access_context;

  // call START callbacks of all plugins
  pip_start();
}

void access_deinit(access_ctx_t access_context) {
  access_ctx_t_ *ctx = (access_ctx_t_ *)access_context;

  // stop and terminate plugins
  pep_term();
}

int access_register_pep_plugin(access_ctx_t access_context, plugin_t *plugin) {
  access_ctx_t_ *ctx = (access_ctx_t_ *)access_context;
  pep_register_plugin(plugin);
}

int access_register_pip_plugin(access_ctx_t access_context, plugin_t *plugin) {
  access_ctx_t_ *ctx = (access_ctx_t_ *)access_context;
  pip_register_plugin(plugin);
}

int access_register_pap_plugin(access_ctx_t access_context, plugin_t *plugin) {
  access_ctx_t_ *ctx = (access_ctx_t_ *)access_context;
  pap_register_plugin(plugin);
}
