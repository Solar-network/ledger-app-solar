#pragma once

#include "buffer.h"

#include "transaction/errors.h"

typedef struct {
    uint64_t amount;  /// amount (8 bytes)
} burn_transaction_asset_t;

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
parser_status_e burn_type_deserialise(buffer_t *buf, burn_transaction_asset_t *tx);
