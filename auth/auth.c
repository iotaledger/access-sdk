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

#include "auth.h"
#include "auth_debug.h"
#include "auth_internal.h"

typedef struct auth_struct auth_struct_t;

static auth_struct_t internal;

static int auth_init(auth_ctx_t *session, void *ext, int type) {
  int ret = AUTH_ERROR;

  if (NULL != session) {
    memset((void *)session, 0, sizeof(auth_ctx_t));

    AUTH_GET_INTERNAL(session) = &internal;

    if (NULL != AUTH_GET_INTERNAL(session)) {
      memset((void *)AUTH_GET_INTERNAL(session), 0, sizeof(auth_struct_t));

      AUTH_GET_INTERNAL_TYPE(session) = type;
      session->ext = ext;

      ret = AUTH_OK;
    }
  }

  return ret;
}

int auth_init_client(auth_ctx_t *session, void *ext) { return auth_init(session, ext, AUTH_TYPE_CLIENT); }

int auth_init_server(auth_ctx_t *session, void *ext) { return auth_init(session, ext, AUTH_TYPE_SERVER); }

int auth_set_option(auth_ctx_t *session, const char *key, unsigned char *value) {
  int ret = AUTH_ERROR;

  if (NULL != session) {
    if (AUTH_TYPE_SERVER == AUTH_GET_INTERNAL_TYPE(session)) {
      ret = auth_internal_server_set_option(session, key, value);
    } else if (AUTH_TYPE_CLIENT == AUTH_GET_INTERNAL_TYPE(session)) {
      ret = auth_internal_client_set_option(session, key, value);
    }
  }

  return ret;
}

int auth_authenticate(auth_ctx_t *session) {
  int ret = AUTH_ERROR;

  if (NULL != session) {
    if (AUTH_TYPE_SERVER == AUTH_GET_INTERNAL_TYPE(session)) {
      ret = auth_internal_server_authenticate(session);
    } else if (AUTH_TYPE_CLIENT == AUTH_GET_INTERNAL_TYPE(session)) {
      ret = auth_internal_client_authenticate(session);
    }
  }

  return ret;
}

int auth_send(auth_ctx_t *session, const unsigned char *data, unsigned short len) {
  int ret = AUTH_ERROR;

  if ((NULL != session) && (NULL != data) && (len > 0)) {
    /* Encrypt and send */
    if (AUTH_TYPE_SERVER == AUTH_GET_INTERNAL_TYPE(session)) {
      ret = auth_internal_server_send(session, data, len);
    } else if (AUTH_TYPE_CLIENT == AUTH_GET_INTERNAL_TYPE(session)) {
      ret = auth_internal_client_send(session, data, len);
    }
  }

  return ret;
}

int auth_receive(auth_ctx_t *session, unsigned char **data, unsigned short *len) {
  int ret = AUTH_ERROR;

  if (NULL != session) {
    /* Decrypt and receive */
    if (AUTH_TYPE_SERVER == AUTH_GET_INTERNAL_TYPE(session)) {
      ret = auth_internal_server_receive(session, data, len);
    } else if (AUTH_TYPE_CLIENT == AUTH_GET_INTERNAL_TYPE(session)) {
      ret = auth_internal_client_receive(session, data, len);
    }
  }

  return ret;
}

int auth_release(auth_ctx_t *session) {
  int ret = AUTH_ERROR;

  if (NULL != AUTH_GET_INTERNAL(session)) {
    if (AUTH_TYPE_SERVER == AUTH_GET_INTERNAL_TYPE(session)) {
      auth_internal_release_server(session);
    } else if (AUTH_TYPE_CLIENT == AUTH_GET_INTERNAL_TYPE(session)) {
      auth_internal_release_client(session);
    }

    AUTH_GET_INTERNAL(session) = NULL;

    ret = AUTH_OK;
  }

  if (NULL != session) {
    session = NULL;

    ret = AUTH_OK;
  }

  return ret;
}
