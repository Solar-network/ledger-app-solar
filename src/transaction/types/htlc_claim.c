/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "htlc_claim.h"
#include "constants.h"
#include "../../common/buffer.h"

parser_status_e htlc_claim_type_deserialise(buffer_t *buf, htlc_claim_transaction_asset_t *tx) {
    // hash type
    if (!buffer_read_u8(buf, &tx->hash_type)) {
        return WRONG_LENGTH_ERROR;
    }

    // lock transaction id
    tx->lockId = (uint8_t *) (buf->ptr + buf->offset);

    if (!buffer_seek_cur(buf, HASH_32_LEN)) {
        return WRONG_LENGTH_ERROR;
    }

    // unlock secret length
    if (!buffer_read_u8(buf, &tx->unlock_secret_length)) {
        return WRONG_LENGTH_ERROR;
    }

    if (tx->unlock_secret_length < 1 || tx->unlock_secret_length > MAX_HTLC_HASH_LEN) {
        return CORE_ASSET_PARSING_ERROR;
    }

    // unlock secret
    tx->unlock_secret = (uint8_t *) (buf->ptr + buf->offset);

    if (!buffer_seek_cur(buf, tx->unlock_secret_length)) {
        return WRONG_LENGTH_ERROR;
    }

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
