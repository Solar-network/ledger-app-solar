/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "burn.h"

#include "buffer.h"

#include "constants.h"

#include "transaction/transaction_errors.h"

parser_status_e burn_type_deserialise(buffer_t *buf, burn_transaction_asset_t *tx) {
    // amount
    if (!buffer_read_u64(buf, &tx->amount, LE)) {
        return WRONG_LENGTH_ERROR;
    }

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
