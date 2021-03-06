/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#pragma once

#include "../errors.h"
#include "../../common/buffer.h"

#define MIN_NUM_TRANSFERS 1
#define MAX_NUM_TRANSFERS 40  // Limited respect to protocol maximum due to SRAM limitations

typedef struct {
    uint16_t transfers_length;  /// length (2 bytes)
    uint8_t *transfers;         /// transfer {address, amount}
} transfer_transaction_asset_t;

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
parser_status_e transfer_type_deserialise(buffer_t *buf, transfer_transaction_asset_t *tx);
