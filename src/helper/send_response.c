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

#include "send_response.h"

#include <stddef.h>  // size_t
#include <stdint.h>  // uint*_t
#include <string.h>  // memmove

#include "buffer.h"

#include "address.h"
#include "constants.h"
#include "context.h"
#include "globals.h"
#include "sw.h"

int helper_send_response_pubkey() {
    uint8_t resp[1 + PUBLIC_KEY_LEN + 1 + CHAINCODE_LEN] = {0};
    size_t offset = 0;

    resp[offset++] = PUBLIC_KEY_LEN;
    memmove(resp + offset, G_context.pk_info.raw_public_key, PUBLIC_KEY_LEN);
    offset += PUBLIC_KEY_LEN;

    if (G_context.pk_info.use_chaincode) {
        resp[offset++] = CHAINCODE_LEN;
        memmove(resp + offset, G_context.pk_info.chain_code, CHAINCODE_LEN);
        offset += CHAINCODE_LEN;
    }

    reset_app_context();

    return io_send_response_pointer(resp, offset, SW_OK);
}

int helper_send_response_address() {
    uint8_t pubkey_hash[ADDRESS_HASH_LEN] = {0};
    char address[ADDRESS_LEN + 1] = {0};

    uint8_t resp[1 + ADDRESS_LEN] = {0};
    size_t offset = 0;

    resp[offset++] = ADDRESS_LEN;
    if (!address_from_pubkey(G_context.pk_info.raw_public_key,
                             pubkey_hash,
                             ADDRESS_HASH_LEN + 1,
                             G_context.network)) {
        return io_send_sw(SW_ENCODE_ADDRESS_FAIL);
    }

    if (base58_encode_address(pubkey_hash, ADDRESS_HASH_LEN, address, ADDRESS_LEN + 1) < 0) {
        return io_send_sw(SW_ENCODE_ADDRESS_FAIL);
    }

    memmove(resp + offset, address, ADDRESS_LEN);
    offset += ADDRESS_LEN;

    reset_app_context();

    return io_send_response_pointer(resp, offset, SW_OK);
}

int helper_send_response_sig() {
    uint8_t resp[SIG_SCHNORR_LEN] = {0};

    memmove(resp, G_context.tx_info.signature, SIG_SCHNORR_LEN);

    return io_send_response_pointer(resp, SIG_SCHNORR_LEN, SW_OK);
}
