#pragma once

#include "buffer.h"

#include "transaction/errors.h"

typedef struct {
    uint8_t ipfs_length;  /// ipfs length (1 byte)
    uint8_t *ipfs;        /// ipfs (MAX 64 bytes)
} ipfs_transaction_asset_t;

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
parser_status_e ipfs_type_deserialise(buffer_t *buf, ipfs_transaction_asset_t *tx);
