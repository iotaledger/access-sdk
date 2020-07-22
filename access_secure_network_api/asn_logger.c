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
 * \file asn_logger.c
 * \brief
 * Logger for Access Secure Network module
 *
 * @Author Bernardo Araujo
 *
 * \notes
 *
 * \history
 * 17.07.2020. Initial version.
 ****************************************************************************/

#include "asn_logger.h"

#define ASN_LOGGER_ID "asn"

logger_id_t asn_logger_id;

void logger_init_asn(logger_level_t level) {
  asn_logger_id = logger_helper_enable(ASN_LOGGER_ID, level, true);
  log_info(asn_logger_id, "[%s:%d] enable logger %s.\n", __func__, __LINE__, ASN_LOGGER_ID);
}

void logger_destroy_asn() {
  log_info(asn_logger_id, "[%s:%d] destroy logger %s.\n", __func__, __LINE__, ASN_LOGGER_ID);
  logger_helper_release(asn_logger_id);
}
