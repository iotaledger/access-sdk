/*
 * This file is part of the Frost distribution
 * (https://github.com/xainag/frost)
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

/****************************************************************************
 * \project IOTA Access
 * \file auth_logger.h
 * \brief
 * Logger for Authentication module
 *
 * @Author Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 17.07.2020. Initial version.
 * 01.08.2020. Renaming.
 ****************************************************************************/


#ifndef AUTH_LOGGER_H
#define AUTH_LOGGER_H

#include "utils/logger_helper.h"

/**
 * @brief logger ID
 *
 */
extern logger_id_t auth_logger_id;

/**
 * @brief init Authentication logger
 *
 * @param[in] level A level of the logger
 *
 */
void logger_init_auth(logger_level_t level);

/**
 * @brief cleanup Authentication logger
 *
 */
void logger_destroy_auth();

#endif  // AUTH_LOGGER_ID
