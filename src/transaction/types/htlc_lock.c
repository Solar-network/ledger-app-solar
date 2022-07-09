/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "htlc_lock.h"
#include "constants.h"
#include "../../common/buffer.h"

parser_status_e htlc_lock_type_deserialise(buffer_t *buf, htlc_lock_transaction_asset_t *tx) {
    // amount
    if (!buffer_read_u64(buf, &tx->amount, LE)) {
        return WRONG_LENGTH_ERROR;
    }

    // secret hash length
    if (!buffer_read_u8(buf, &tx->secret_hash_length)) {
        return WRONG_LENGTH_ERROR;
    }

    if (tx->secret_hash_length < 1 || tx->secret_hash_length > MAX_HTLC_HASH_LEN) {
        return CORE_ASSET_PARSING_ERROR;
    }

    // secret hash
    tx->secret_hash = (uint8_t *) (buf->ptr + buf->offset);

    if (!buffer_seek_cur(buf, tx->secret_hash_length)) {
        return WRONG_LENGTH_ERROR;
    }

    // expiration type
    if (!buffer_read_u8(buf, &tx->expiration_type)) {
        return WRONG_LENGTH_ERROR;
    }

    // expiration value
    if (!buffer_read_u32(buf, &tx->expiration_value, LE)) {
        return WRONG_LENGTH_ERROR;
    }

    // recipientId
    tx->recipientId = (uint8_t *) (buf->ptr + buf->offset);

    if (!buffer_seek_cur(buf, ADDRESS_HASH_LEN)) {
        return WRONG_LENGTH_ERROR;
    }

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
