#pragma once

#include <stdint.h>  // uint*_t

#include "buffer.h"

#include "transaction/transaction_errors.h"

#define MAX_NUM_VOTES       53
#define MIN_USERNAME_LENGTH 1
#define MAX_USERNAME_LENGTH 20
#define MAX_PERCENTAGE      10000

typedef struct {
    uint8_t username_length;  /// Length of the username (1 byte)
    uint8_t *username;        /// Username (1-20 bytes)
    uint16_t percentage;      /// vote percentage (2 bytes)
} vote_deserialised_t;

typedef struct {
    uint8_t vote_length;  /// length (1 byte)
    uint8_t *votes;       /// vote {username, percentage}
} vote_transaction_asset_t;

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
parser_status_e vote_type_deserialise(buffer_t *buf, vote_transaction_asset_t *tx);
