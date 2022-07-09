/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "multi_signature_registration.h"
#include "constants.h"
#include "../../common/buffer.h"

parser_status_e multisignature_type_deserialise(buffer_t *buf,
                                                multisignature_transaction_asset_t *tx) {
    // min
    if (!buffer_read_u8(buf, &tx->min)) {
        return WRONG_LENGTH_ERROR;
    }

    // count
    if (!buffer_read_u8(buf, &tx->pkey_length)) {
        return WRONG_LENGTH_ERROR;
    }

    if (tx->pkey_length < MIN_NUM_SIGNATURES || tx->pkey_length > MAX_NUM_SIGNATURES) {
        return CORE_ASSET_PARSING_ERROR;
    }

    // public keys
    tx->pkeys = (uint8_t *) (buf->ptr + buf->offset);

    if (!buffer_seek_cur(buf, PUBLIC_KEY_LEN * tx->pkey_length)) {
        return WRONG_LENGTH_ERROR;
    }

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
