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

#ifndef _ACCESS_H_
#define _ACCESS_H_

#include "plugin.h"
#include "wallet.h"

typedef void *access_ctx_t;

void access_init(access_ctx_t *access_context);

void access_deinit(access_ctx_t access_context);

int access_register_pep_plugin(access_ctx_t access_context, plugin_t *plugin);

int access_register_pip_plugin(access_ctx_t access_context, plugin_t *plugin);

int access_register_pap_plugin(access_ctx_t access_context, plugin_t *plugin);

#endif
