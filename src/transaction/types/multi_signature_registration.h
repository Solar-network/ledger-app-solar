/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#pragma once

#include "../errors.h"
#include "../../common/buffer.h"

#define MIN_NUM_SIGNATURES 2
#define MAX_NUM_SIGNATURES 16

typedef struct {
    uint8_t min;          /// multisignatures minimum (1 byte)
    uint8_t pkey_length;  /// number of public keys (1 byte)
    uint8_t *pkeys;       /// publickeys (MAX 33 * 16 bytes)
} multisignature_transaction_asset_t;

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
parser_status_e multisignature_type_deserialise(buffer_t *buf,
                                                multisignature_transaction_asset_t *tx);
