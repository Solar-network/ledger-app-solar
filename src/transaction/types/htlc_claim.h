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
    uint8_t hash_type;             /// hashing algorithm (1 byte)
    uint8_t *lockId;               /// lock transaction id (32 bytes)
    uint8_t unlock_secret_length;  /// lock secret length (1 byte)
    uint8_t *unlock_secret;        /// unlock secret (1-255 bytes)
} htlc_claim_transaction_asset_t;

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
parser_status_e htlc_claim_type_deserialise(buffer_t *buf, htlc_claim_transaction_asset_t *tx);
