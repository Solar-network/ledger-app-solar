/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "ipfs.h"

#include "buffer.h"

#include "constants.h"

#include "transaction/errors.h"

parser_status_e ipfs_type_deserialise(buffer_t *buf, ipfs_transaction_asset_t *tx) {
    // ipfs
    tx->ipfs = (uint8_t *) (buf->ptr + buf->offset);

    // hash function
    if (!buffer_seek_cur(buf, 1)) {
        return WRONG_LENGTH_ERROR;
    }

    // length
    if (!buffer_read_u8(buf, &tx->ipfs_length)) {
        return WRONG_LENGTH_ERROR;
    }

    if (tx->ipfs_length < 1 || tx->ipfs_length > HASH_64_LEN) {
        return CORE_ASSET_PARSING_ERROR;
    }

    if (!buffer_seek_cur(buf, tx->ipfs_length)) {
        return WRONG_LENGTH_ERROR;
    }

    tx->ipfs_length += 2;

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
