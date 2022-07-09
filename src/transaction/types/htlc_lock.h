/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#pragma once

#include "../errors.h"
#include "../../common/buffer.h"

typedef struct {
    uint64_t amount;             /// Amount (8 bytes)
    uint8_t secret_hash_length;  /// Secret hash length (1 byte)
    uint8_t *secret_hash;        /// secret hash (1-255 bytes)
    uint8_t expiration_type;     /// expiration type (1 byte)
    uint32_t expiration_value;   /// expiration value (4 bytes)
    uint8_t *recipientId;        /// recipientId (MAX 21 bytes)
} htlc_lock_transaction_asset_t;

/**
 * Deserialise asset of transaction in structure.
 *
 * @param[in, out] buf
 *   Pointer to buffer with serialised transaction.
 * @param[out]     tx
 *   Pointer to transaction asset structure.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */
parser_status_e htlc_lock_type_deserialise(buffer_t *buf, htlc_lock_transaction_asset_t *tx);
