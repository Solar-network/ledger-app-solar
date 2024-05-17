/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "vote.h"

#include <stdint.h>  // uint*_t

#include "buffer.h"

#include "constants.h"

#include "transaction/transaction_errors.h"
#include "transaction/transaction_utils.h"

parser_status_e vote_type_deserialise(buffer_t *buf, vote_transaction_asset_t *tx) {
    // length
    if (!buffer_read_u8(buf, &tx->vote_length)) {
        return WRONG_LENGTH_ERROR;
    }

    if (tx->vote_length > MAX_NUM_VOTES) {
        return CORE_ASSET_PARSING_ERROR;
    }

    // votes
    tx->votes = (uint8_t *) (buf->ptr + buf->offset);
    uint16_t total_percentage = 0;

    for (uint8_t i = 0; i < tx->vote_length; i++) {
        uint8_t username_length = 0;
        if (!buffer_read_u8(buf, &username_length)) {
            return WRONG_LENGTH_ERROR;
        }

        if (username_length < MIN_USERNAME_LENGTH || username_length > MAX_USERNAME_LENGTH) {
            return CORE_ASSET_PARSING_ERROR;
        }

        uint8_t *username = (uint8_t *) (buf->ptr + buf->offset);

        if (!buffer_seek_cur(buf, username_length)) {
            return WRONG_LENGTH_ERROR;
        }

        if (!transaction_utils_check_ascii(username, username_length, false)) {
            return CORE_ASSET_PARSING_ERROR;
        }

        uint16_t percentage = 0;

        if (!buffer_read_u16(buf, &percentage, LE)) {
            return WRONG_LENGTH_ERROR;
        }

        if (percentage < 1 || percentage > 10000) {
            return CORE_ASSET_PARSING_ERROR;
        }

        total_percentage += percentage;
    }

    if (tx->vote_length > 0 && total_percentage != MAX_PERCENTAGE) {
        return CORE_ASSET_PARSING_ERROR;
    }

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
