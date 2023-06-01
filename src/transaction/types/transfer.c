/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "transfer.h"

#include "buffer.h"

#include "constants.h"

#include "transaction/errors.h"

parser_status_e transfer_type_deserialise(buffer_t *buf, transfer_transaction_asset_t *tx) {
    // length
    if (!buffer_read_u16(buf, &tx->transfers_length, LE)) {
        return WRONG_LENGTH_ERROR;
    }

    if (tx->transfers_length < MIN_NUM_TRANSFERS || tx->transfers_length > MAX_NUM_TRANSFERS) {
        return CORE_ASSET_PARSING_ERROR;
    }

    // Transfers
    tx->transfers = (uint8_t *) (buf->ptr + buf->offset);

    if (!buffer_seek_cur(buf, (ADDRESS_HASH_LEN + 8) * tx->transfers_length)) {
        return WRONG_LENGTH_ERROR;
    }

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
