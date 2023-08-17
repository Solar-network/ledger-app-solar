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

#include "address.h"

#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <stdint.h>   // uint*_t
#include <string.h>   // memmove

#include "base58.h"
#include "cx.h"
#include "os.h"

#include "constants.h"
#include "sw.h"

bool address_from_pubkey(const uint8_t public_key[static 33],
                         uint8_t *out,
                         size_t out_len,
                         uint8_t network) {
    uint8_t address[ADDRESS_HASH_LEN] = {0};
    cx_ripemd160_t ctx;

    if (out_len <= ADDRESS_HASH_LEN) {
        return false;
    }

    if (cx_ripemd160_init_no_throw(&ctx) != CX_OK) {
        return false;
    }

    if (cx_hash_no_throw((cx_hash_t *) &ctx,
                         CX_LAST,
                         public_key,
                         PUBLIC_KEY_LEN,
                         address,
                         ADDRESS_HASH_LEN) != CX_OK) {
        return false;
    }

    memmove(out + 1, address, ADDRESS_HASH_LEN - 1);
    out[0] = network;

    return true;
}

void crypto_get_checksum(const uint8_t *in, size_t in_len, uint8_t out[static 4]) {
    uint8_t buffer[HASH_32_LEN];
    cx_hash_sha256(in, in_len, buffer, HASH_32_LEN);
    cx_hash_sha256(buffer, HASH_32_LEN, buffer, HASH_32_LEN);
    memmove(out, buffer, 4);
}

int base58_encode_address(const uint8_t *in, size_t in_len, char *out, size_t out_len) {
    if (in_len != ADDRESS_HASH_LEN) {
        return -1;
    }
    uint8_t tmp[ADDRESS_HASH_LEN + 4 + 1];  // ... + checksum + null-byte

    memcpy(tmp, in, in_len);
    crypto_get_checksum(tmp, in_len, tmp + in_len);
    size_t len = base58_encode(tmp, in_len + 4, out, out_len - 1);
    if (len < 1) {
        return -1;
    }
    out[len++] = '\0';
    return len;
}
