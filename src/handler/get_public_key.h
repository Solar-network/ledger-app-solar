/*****************************************************************************
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *
 *  This software also incorporates work covered by the following copyright
 *  and permission notice:
 *
 *   Ledger App Boilerplate.
 *   (c) 2020 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#pragma once

#include <stdbool.h>  // bool

#include "buffer.h"

/**
 * Handler for GET_PUBLIC_KEY command. If it successfully parses BIP32 path,
 * derive public key/chain code and send APDU response.
 *
 * @see G_context.bip32_path, G_context.pk_info.raw_public_key and
 *      G_context.pk_info.chain_code.
 *
 * @param[in,out] cdata
 *   Command data with BIP32 path.
 * @param[in]     user_approval
 *   0 for not displaying on screen, 1 otherwise.
 * @param[in]     use_chain_code
 *   0 for not requesting the chain code, 1 otherwise.
 *
 * @return zero or positive integer if success, negative integer otherwise.
 *
 */
int handler_get_public_key(buffer_t *cdata, bool user_approval, bool use_chain_code);
