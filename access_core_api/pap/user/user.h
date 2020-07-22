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

#ifndef _USER_H_
#define _USER_H_

#include "user_implementation.h"

int user_init();
void user_deinit();
void user_get_all(char* json_response);
void user_get_obj(const char* username, char* json_response);
void user_put_obj(const char* user_object, char* json_response);
void user_get_user_id(const char* username, char* json_response);
void user_clear_all(char* json_response);

#endif
