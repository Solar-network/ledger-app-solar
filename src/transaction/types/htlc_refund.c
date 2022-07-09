/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "htlc_refund.h"
#include "constants.h"
#include "../../common/buffer.h"

parser_status_e htlc_refund_type_deserialise(buffer_t *buf, htlc_refund_transaction_asset_t *tx) {
    // lock transaction id
    tx->lockId = (uint8_t *) (buf->ptr + buf->offset);

    if (!buffer_seek_cur(buf, HASH_32_LEN)) {
        return WRONG_LENGTH_ERROR;
    }

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
