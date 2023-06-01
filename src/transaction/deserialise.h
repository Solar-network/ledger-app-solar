#pragma once

#include <stdint.h>  // uint*_t

#include "buffer.h"

#include "transaction/types.h"

/**
 * Deserialise raw transaction in structure.
 *
 * @param[in, out] buf
 *   Pointer to buffer with serialised transaction.
 * @param[out]     tx
 *   Pointer to transaction structure.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */
parser_status_e transaction_deserialise(buffer_t *buf, transaction_t *tx);

/**
 * Deserialise common part of the raw transaction.
 *
 * @param[in, out] buf
 *   Pointer to buffer with serialised transaction.
 * @param[out]     tx
 *   Pointer to transaction structure.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */
parser_status_e transaction_deserialise_common(buffer_t *buf, transaction_t *tx);

/**
 * Deserialise transaction specific asset of the raw transaction.
 *
 * @param[in, out] buf
 *   Pointer to buffer with serialised transaction.
 * @param[out]     tx
 *   Pointer to transaction assets structure.
 * @param[in]     type
 *   Transaction type.
 * @param[in]     typeGroup
 *   Transaction typeGroup.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */
parser_status_e transaction_deserialise_core_asset(buffer_t *buf,
                                                   transaction_asset_t *tx,
                                                   uint16_t type,
                                                   uint32_t typeGroup);

/**
 * Deserialise message in structure.
 *
 * @param[in, out] buf
 *   Pointer to buffer with message.
 * @param[out]     tx
 *   Pointer to transaction structure.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */
parser_status_e message_deserialise(buffer_t *buf, transaction_t *tx);
