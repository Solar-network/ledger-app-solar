/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *
 *****************************************************************************
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *
 *  This software also incorporates work covered by the following copyright
 *  and permission notice:
 *
 *   Ledger App Boilerplate.
 *   (c) Ledger SAS.
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

#include "get_public_key.h"

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <string.h>   // memset, explicit_bzero

#include "buffer.h"
#include "cx.h"
#include "io.h"
#include "os.h"

#include "context.h"
#include "globals.h"
#include "types.h"
#include "sw.h"

#include "crypto/crypto.h"
#include "helper/send_response.h"
#include "ui/display.h"

int handler_get_public_key(buffer_t *cdata, bool user_approval, bool use_chain_code) {
    reset_app_context();
    G_context.req_type = CONFIRM_PUBLICKEY;
    G_context.pk_info.use_chaincode = use_chain_code;

    cx_ecfp_private_key_t private_key = {0};
    cx_ecfp_public_key_t public_key = {0};

    if (!buffer_read_u8(cdata, &G_context.bip32_path_len) ||
        !buffer_read_bip32_path(cdata, G_context.bip32_path, (size_t) G_context.bip32_path_len)) {
        return io_send_sw(SW_WRONG_DATA_LENGTH);
    }

    // derive private key according to BIP32 path
    crypto_derive_private_key(&private_key,
                              use_chain_code ? G_context.pk_info.chain_code : NULL,
                              G_context.bip32_path,
                              G_context.bip32_path_len);

    // generate corresponding public key
    crypto_init_public_key(&private_key, &public_key, G_context.pk_info.raw_public_key);

    // reset private key
    explicit_bzero(&private_key, sizeof(private_key));

    if (user_approval) {
        return ui_display_public_key();
    }

    return helper_send_response_pubkey();
}
