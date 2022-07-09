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
    uint8_t *lockId;  /// lock transaction id (32 bytes)
} htlc_refund_transaction_asset_t;

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
parser_status_e htlc_refund_type_deserialise(buffer_t *buf, htlc_refund_transaction_asset_t *tx);
