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

#include "apdu/dispatcher.h"

#include <stdint.h>
#include <stdbool.h>

#include "buffer.h"
#include "io.h"

#include "constants.h"
#include "sw.h"
#include "types.h"

#include "handler/get_version.h"
#include "handler/get_app_name.h"
#include "handler/get_public_key.h"
#include "handler/get_address.h"
#include "handler/sign_tx.h"

int apdu_dispatcher(const command_t *cmd) {
    if (cmd->cla != CLA) {
        return io_send_sw(SW_CLA_NOT_SUPPORTED);
    }

    buffer_t buf = {0};

    switch (cmd->ins) {
        case GET_VERSION:
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            return handler_get_version();

        case GET_APP_NAME:
            if (cmd->p1 != 0 || cmd->p2 != 0) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            return handler_get_app_name();

        case GET_PUBLIC_KEY:
            // P1:
            // - 0x00: No user approval
            // - 0x01: User approval
            // P2:
            // - 0x00: Don't request chain code
            // - 0x01: Request chain code
            if (cmd->p1 > 1 || cmd->p2 > 1) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            if (!cmd->data) {
                return io_send_sw(SW_WRONG_DATA_LENGTH);
            }

            buf.ptr = cmd->data;
            buf.size = cmd->lc;
            buf.offset = 0;

            return handler_get_public_key(&buf, cmd->p1, cmd->p2);

        case GET_ADDRESS:
            // P1:
            // - 0x00: No user approval
            // - 0x01: User approval
            // P2:
            // - 0x3F (60): Solar Mainnet
            // - 0x1E (30): Solar Testnet
            if (cmd->p1 > 1 ||
                (cmd->p2 != NETWORK_SOLAR_MAINNET && cmd->p2 != NETWORK_SOLAR_TESTNET)) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            if (!cmd->data) {
                return io_send_sw(SW_WRONG_DATA_LENGTH);
            }

            buf.ptr = cmd->data;
            buf.size = cmd->lc;
            buf.offset = 0;

            return handler_get_address(&buf, cmd->p1 == 1, cmd->p2);

        case SIGN_MESSAGE:
        case SIGN_TX:
            if ((cmd->p1 == P1_START && cmd->p2 != P2_MORE) || cmd->p1 > P1_MAX ||
                (cmd->p2 != P2_LAST && cmd->p2 != P2_MORE)) {
                return io_send_sw(SW_WRONG_P1P2);
            }

            if (!cmd->data) {
                return io_send_sw(SW_WRONG_DATA_LENGTH);
            }

            buf.ptr = cmd->data;
            buf.size = cmd->lc;
            buf.offset = 0;

            return handler_sign_tx(&buf, cmd->p1, cmd->p2, cmd->ins == SIGN_MESSAGE);

        default:
            return io_send_sw(SW_INS_NOT_SUPPORTED);
    }
}
