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

#include "ui/action/validate.h"

#include <stdbool.h>  // bool

#include "io.h"

#include "context.h"
#include "globals.h"
#include "sw.h"

#include "crypto/crypto.h"
#include "helper/send_response.h"
#include "ui/menu.h"

void ui_action_validate_pubkey(bool choice) {
    if (choice) {
        helper_send_response_pubkey();
    } else {
        io_send_sw(SW_DENY);
    }

    ui_menu_main();
}

void ui_action_validate_address(bool choice) {
    if (choice) {
        helper_send_response_address();
    } else {
        io_send_sw(SW_DENY);
    }

    ui_menu_main();
}

void ui_action_validate_transaction(bool choice) {
    if (choice) {
        G_context.state = STATE_APPROVED;

        if (crypto_sign_message() < 0) {
            io_send_sw(SW_SIGNATURE_FAIL);
        } else {
            helper_send_response_sig();
        }
    } else {
        io_send_sw(SW_DENY);
    }
    reset_app_context();
    ui_menu_main();
}
