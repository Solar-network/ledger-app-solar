/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *
 *****************************************************************************
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *
 *  This software also incorporates work covered by the following copyright
 *  and permission notice:
 *
 *   Ledger App Boilerplate.
 *   (c) 2023 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include "deserialise.h"

#include <stdint.h>  // uint*_t

#include "buffer.h"

#include "constants.h"

#include "transaction/transaction_errors.h"
#include "transaction/transaction_utils.h"
#include "transaction/types.h"

parser_status_e transaction_deserialise(buffer_t *buf, transaction_t *tx) {
    parser_status_e parse_common = transaction_deserialise_common(buf, tx);

    if (parse_common != PARSING_OK) {
        return parse_common;
    }

    return transaction_deserialise_core_asset(buf, &tx->core_asset, tx->type, tx->typeGroup);
}

parser_status_e transaction_deserialise_common(buffer_t *buf, transaction_t *tx) {
    // starting byte
    uint8_t starting_byte;
    if (!buffer_read_u8(buf, &starting_byte)) {
        return WRONG_LENGTH_ERROR;
    }

    if (starting_byte != STARTING_BYTE) {
        return STARTING_BYTE_PARSING_ERROR;
    }

    // version
    uint8_t version;
    if (!buffer_read_u8(buf, &version)) {
        return WRONG_LENGTH_ERROR;
    }

    if (version != TRANSACTION_VERSION_3) {
        return VERSION_PARSING_ERROR;
    }

    // network
    uint8_t network;
    if (!buffer_read_u8(buf, &network)) {
        return WRONG_LENGTH_ERROR;
    }

    if (network != NETWORK_SOLAR_MAINNET && network != NETWORK_SOLAR_TESTNET) {
        return NETWORK_PARSING_ERROR;
    }

    // typeGroup
    if (!buffer_read_u32(buf, &tx->typeGroup, LE)) {
        return WRONG_LENGTH_ERROR;
    }

    // type
    if (!buffer_read_u16(buf, &tx->type, LE)) {
        return WRONG_LENGTH_ERROR;
    }

    // nonce
    if (!buffer_seek_cur(buf, sizeof(uint64_t))) {
        return WRONG_LENGTH_ERROR;
    }

    tx->sender_publickey = (uint8_t *) (buf->ptr + buf->offset);

    // Sender publickey
    if (!buffer_seek_cur(buf, PUBLIC_KEY_LEN)) {
        return WRONG_LENGTH_ERROR;
    }

    // fee
    if (!buffer_read_u64(buf, &tx->fee, LE)) {
        return WRONG_LENGTH_ERROR;
    }

    // memo length
    if (!buffer_read_u8(buf, &tx->memo_len)) {
        return WRONG_LENGTH_ERROR;
    }

    // memo
    tx->memo = (uint8_t *) (buf->ptr + buf->offset);

    if (!buffer_seek_cur(buf, tx->memo_len)) {
        return WRONG_LENGTH_ERROR;
    }

    if (!transaction_utils_check_ascii(tx->memo, tx->memo_len, false)) {
        return MEMO_ENCODING_ERROR;
    }

    return PARSING_OK;
}

parser_status_e transaction_deserialise_core_asset(buffer_t *buf,
                                                   transaction_asset_t *tx,
                                                   uint16_t type,
                                                   uint32_t typeGroup) {
    if (typeGroup == TYPEGROUP_SOLAR) {
        switch (type) {
            case BURN:
                return burn_type_deserialise(buf, &tx->Burn);
            case VOTE:
                return vote_type_deserialise(buf, &tx->Vote);
            default:
                return TYPE_PARSING_ERROR;
        }
    } else if (typeGroup == TYPEGROUP_CORE) {
        switch (type) {
            case IPFS:
                return ipfs_type_deserialise(buf, &tx->Ipfs);
            case TRANSFER:
                return transfer_type_deserialise(buf, &tx->Transfer);
            default:
                return TYPE_PARSING_ERROR;
        }
    }

    return TYPE_PARSING_ERROR;
}

parser_status_e message_deserialise(buffer_t *buf, transaction_t *tx) {
    // message length
    if (!buffer_read_u16(buf, &tx->message_length, LE) || tx->message_length < 1 ||
        tx->message_length > TRANSACTION_MAX_LEN - 3) {
        return WRONG_LENGTH_ERROR;
    }

    tx->message = (uint8_t *) (buf->ptr + buf->offset);

    if (!buffer_seek_cur(buf, tx->message_length)) {
        return WRONG_LENGTH_ERROR;
    }

    if (!transaction_utils_check_ascii(tx->message, tx->message_length, true)) {
        return MESSAGE_ENCODING_ERROR;
    }

    tx->message[tx->message_length] = '\0';

    return (buf->offset == buf->size) ? PARSING_OK : WRONG_LENGTH_ERROR;
}
