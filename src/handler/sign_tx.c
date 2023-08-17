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
 *   (c) Ledger SAS.
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

#include "sign_tx.h"

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <string.h>   // memset, explicit_bzero

#include "cx.h"
#include "os.h"
#include "sw.h"

#include "buffer.h"

#include "context.h"
#include "crypto.h"
#include "globals.h"

#include "transaction/deserialise.h"
#include "transaction/types.h"
#include "ui/display.h"

int handler_sign_tx(buffer_t *cdata, uint8_t chunk, bool more, bool is_message) {
    if (chunk == 0) {  // first APDU, parse BIP32 path
        reset_app_context();
        if (is_message) {
            G_context.req_type = CONFIRM_MESSAGE;
        } else {
            G_context.req_type = CONFIRM_TRANSACTION;
        }

        if (!buffer_read_u8(cdata, &G_context.bip32_path_len) ||
            !buffer_read_bip32_path(cdata,
                                    G_context.bip32_path,
                                    (size_t) G_context.bip32_path_len)) {
            return io_send_sw(SW_WRONG_DATA_LENGTH);
        }

        return io_send_sw(SW_OK);
    } else {  // parse transaction
        if (!is_message && G_context.req_type != CONFIRM_TRANSACTION) {
            return io_send_sw(SW_BAD_STATE);
        }
        if (is_message && G_context.req_type != CONFIRM_MESSAGE) {
            return io_send_sw(SW_BAD_STATE);
        }

        if (G_context.req_num + 1 != chunk) {
            return io_send_sw(SW_OUT_OF_ORDER_REQ);
        }

        G_context.req_num++;

        if (more) {  // more APDUs with transaction part
            if (G_context.tx_info.raw_tx_len + cdata->size > TRANSACTION_MAX_LEN ||  //
                cdata->size < UINT8_MAX ||                                           //
                !buffer_move(cdata,
                             G_context.tx_info.raw_tx + G_context.tx_info.raw_tx_len,
                             cdata->size)) {
                return io_send_sw(SW_WRONG_TX_LENGTH);
            }

            G_context.tx_info.raw_tx_len += cdata->size;

            return io_send_sw(SW_OK);
        } else {  // last APDU, let's parse and sign
            if (G_context.tx_info.raw_tx_len + cdata->size > TRANSACTION_MAX_LEN ||  //
                !buffer_move(cdata,
                             G_context.tx_info.raw_tx + G_context.tx_info.raw_tx_len,
                             cdata->size)) {
                return io_send_sw(SW_WRONG_TX_LENGTH);
            }

            G_context.tx_info.raw_tx_len += cdata->size;

            buffer_t buf = {.ptr = G_context.tx_info.raw_tx,
                            .size = G_context.tx_info.raw_tx_len,
                            .offset = 0};

            parser_status_e status;
            if (G_context.req_type == CONFIRM_TRANSACTION) {
                status = transaction_deserialise(&buf, &G_context.tx_info.transaction);
            } else {
                status = message_deserialise(&buf, &G_context.tx_info.transaction);
            }
            PRINTF("Parsing status: %d.\n", status);
            if (status != PARSING_OK) {
                return io_send_sw(SW_TX_PARSING_FAIL);
            }

            G_context.state = STATE_PARSED;

            cx_sha256_t sha256;

            if (cx_sha256_init_no_throw(&sha256) != CX_OK) {
                return io_send_sw(SW_TX_HASH_FAIL);
            }

            if (cx_hash_no_throw((cx_hash_t *) &sha256,
                                 CX_LAST,
                                 G_context.tx_info.raw_tx,
                                 G_context.tx_info.raw_tx_len,
                                 G_context.tx_info.m_hash,
                                 HASH_32_LEN) != CX_OK) {
                return io_send_sw(SW_TX_HASH_FAIL);
            }

            if (G_context.req_type == CONFIRM_TRANSACTION) {
                return ui_display_transaction();
            }
            return ui_display_message();
        }
    }

    return 0;
}
