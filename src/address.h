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

#include <stdint.h>   // uint*_t
#include <stddef.h>   // size_t
#include <stdbool.h>  // bool

/**
 * Convert public key to address.
 *
 * address = Keccak256(public_key)[12:32] (20 bytes)
 *
 * @param[in]  public_key
 *   Pointer to byte buffer with public key.
 *   The public key is represented as 64 bytes with 32 bytes for
 *   each coordinate.
 * @param[out] out
 *   Pointer to output byte buffer for address.
 * @param[in]  out_len
 *   Length of output byte buffer.
 * @param[in]  network
 *   Network byte.
 *
 * @return true if success, false otherwise.
 *
 */
bool address_from_pubkey(const uint8_t public_key[static 33],
                         uint8_t *out,
                         size_t out_len,
                         uint8_t network);

/**
 * Calculate address checksum.
 *
 * address = Keccak256(public_key)[12:32] (20 bytes)
 *
 * @param[in]  in
 *   Pointer to address bytes.
 * @param[in]  in_len
 *   Length of input byte buffer.
 * @param[out] out
 *   Pointer to output checksum bytes.
 *
 */
void crypto_get_checksum(const uint8_t *in, size_t in_len, uint8_t out[static 4]);

/**
 * Convert address bytes to base58 address.
 *
 *
 * @param[in]  in
 *   Pointer to byte buffer with address.
 * @param[in]  in_len
 *   Length of input address bytes.
 * @param[out] out
 *   Pointer to output byte buffer for address.
 * @param[in]  out_len
 *   Maximum length to write in output byte buffer.
 *
 * @return number of bytes decoded, -1 otherwise.
 *
 */
int base58_encode_address(const uint8_t *in, size_t in_len, char *out, size_t out_len);
