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

#include "ui/display.h"

#include <stdbool.h>  // bool
#include <string.h>   // memset

#include "glyphs.h"

#include "bip32.h"
#include "buffer.h"
#include "format.h"
#include "io.h"
#include "os.h"
#include "ux.h"

#include "address.h"
#include "constants.h"
#include "globals.h"
#include "sw.h"

#include "action/validate.h"
#include "transaction/types.h"
#include "ui/ctx.h"
#include "ui/transactions/ipfs_display.h"
#include "ui/transactions/transfer_display.h"
#include "ui/transactions/burn_display.h"
#include "ui/transactions/vote_display.h"

ctx_t display_context = {0};

enum e_state {
    STATIC_SCREEN,
    DYNAMIC_SCREEN,
    MEMO_SCREEN,
} current_state = STATIC_SCREEN;

static action_validate_cb g_validate_callback;

static char g_transaction_name[25];

static char g_current_title[MAX_TITLE_LEN];
static char g_current_text[MAX_TEXT_LEN];

// Step with approve button
UX_STEP_CB(ux_display_approve_step,
           pb,
           (*g_validate_callback)(true),
           {
               &C_icon_validate_14,
               "Approve",
           });
// Step with reject button
UX_STEP_CB(ux_display_reject_step,
           pb,
           (*g_validate_callback)(false),
           {
               &C_icon_crossmark,
               "Reject",
           });

// Display Transaction first step
UX_STEP_NOCB(ux_display_review_step,
             pnn,
             {
                 &C_icon_eye,
                 "Review",
                 g_transaction_name,
             });

/**
 * PublicKey UX Components
 *
 */

// initial step for publicKey confirmation.
UX_STEP_NOCB(ux_display_confirm_public_key_step, pn, {&C_icon_eye, "Confirm Public Key"});

// Step with title/text for public key
UX_STEP_NOCB(ux_display_public_key_step,
             bnnn_paging,
             {
                 .title = "Public Key",
                 .text = g_current_text,
             });

// FLOW to display address and BIP32 path:
// #1 screen: eye icon + "Confirm PublicKey"
// #2 screen: display public key
// #3 screen: approve button
// #4 screen: reject button
UX_FLOW(ux_display_pubkey_flow,
        &ux_display_confirm_public_key_step,
        &ux_display_public_key_step,
        &ux_display_approve_step,
        &ux_display_reject_step);

int ui_display_public_key() {
    if (G_context.req_type != CONFIRM_PUBLICKEY || G_context.state != STATE_NONE) {
        G_context.state = STATE_NONE;
        return io_send_sw(SW_BAD_STATE);
    }

    if (format_hex(G_context.pk_info.raw_public_key,
                   PUBLIC_KEY_LEN,
                   g_current_text,
                   MAX_TEXT_LEN) == -1) {
        return io_send_sw(SW_DISPLAY_PUBLICKEY_FAIL);
    }

    g_validate_callback = &ui_action_validate_pubkey;

    ux_flow_init(0, ux_display_pubkey_flow, NULL);

    return 0;
}

/**
 * Address UX Components
 *
 */

// initial step for address confirmation.
UX_STEP_NOCB(ux_display_confirm_addr_step, pn, {&C_icon_eye, "Confirm Address"});

// Step with title/text for address
UX_STEP_NOCB(ux_display_address_step,
             bnnn_paging,
             {
                 .title = "Address",
                 .text = g_current_text,
             });

// FLOW to display address and BIP32 path:
// #1 screen: eye icon + "Confirm Address"
// #2 screen: display address
// #3 screen: approve button
// #4 screen: reject button
UX_FLOW(ux_display_address_flow,
        &ux_display_confirm_addr_step,
        &ux_display_address_step,
        &ux_display_approve_step,
        &ux_display_reject_step);

int ui_display_address() {
    if (G_context.req_type != CONFIRM_ADDRESS || G_context.state != STATE_NONE) {
        G_context.state = STATE_NONE;
        return io_send_sw(SW_BAD_STATE);
    }

    memset(g_current_text, 0, MAX_TEXT_LEN);
    uint8_t address[ADDRESS_HASH_LEN] = {0};

    if (!address_from_pubkey(G_context.pk_info.raw_public_key,
                             address,
                             ADDRESS_HASH_LEN + 1,
                             G_context.network)) {
        return io_send_sw(SW_DISPLAY_ADDRESS_FAIL);
    }

    base58_encode_address(address, ADDRESS_HASH_LEN, g_current_text, MAX_TEXT_LEN);

    g_validate_callback = &ui_action_validate_address;

    ux_flow_init(0, ux_display_address_flow, NULL);

    return 0;
}

void bnnn_paging_edgecase() {
    G_ux.flow_stack[G_ux.stack_count - 1].prev_index =
        G_ux.flow_stack[G_ux.stack_count - 1].index - 2;
    G_ux.flow_stack[G_ux.stack_count - 1].index--;
    ux_flow_relayout();
}

// Function to handle Dynamic Screen
// Three states: STATIC, DYNAMIC and MEMO
void display_next_state(bool is_upper_delimiter) {
    if (is_upper_delimiter) {
        if (current_state == STATIC_SCREEN) {
            if (context_get_next(&display_context,
                                 &G_context.tx_info.transaction,
                                 g_current_title,
                                 g_current_text)) {
                current_state = DYNAMIC_SCREEN;
            }

            ux_flow_next();
        } else {
            if (context_get_previous(&display_context,
                                     &G_context.tx_info.transaction,
                                     g_current_title,
                                     g_current_text)) {
                current_state = DYNAMIC_SCREEN;
                ux_flow_next();
            } else {
                current_state = STATIC_SCREEN;
                ux_flow_prev();
            }
        }
    } else {
        if (current_state == STATIC_SCREEN) {
            if (G_context.tx_info.transaction.memo_len > 0) {
                snprintf(g_current_title, sizeof(g_current_title), "%s", "Memo");
                snprintf(g_current_text,
                         sizeof(g_current_text),
                         "%.*s",
                         G_context.tx_info.transaction.memo_len,
                         G_context.tx_info.transaction.memo);
                current_state = MEMO_SCREEN;
            } else if (context_get_previous(&display_context,
                                            &G_context.tx_info.transaction,
                                            g_current_title,
                                            g_current_text)) {
                current_state = DYNAMIC_SCREEN;
            }

            bnnn_paging_edgecase();
        } else if (current_state == DYNAMIC_SCREEN) {
            if (context_get_next(&display_context,
                                 &G_context.tx_info.transaction,
                                 g_current_title,
                                 g_current_text)) {
                bnnn_paging_edgecase();
            } else if (G_context.tx_info.transaction.memo_len > 0) {
                snprintf(g_current_title, sizeof(g_current_title), "%s", "Memo");
                snprintf(g_current_text,
                         sizeof(g_current_text),
                         "%.*s",
                         G_context.tx_info.transaction.memo_len,
                         G_context.tx_info.transaction.memo);
                current_state = MEMO_SCREEN;
                bnnn_paging_edgecase();
            } else {
                current_state = STATIC_SCREEN;
                ux_flow_next();
            }
        } else {
            current_state = STATIC_SCREEN;
            ux_flow_next();
        }
    }
}

// Upper delimiter step
UX_STEP_INIT(step_upper_delimiter, NULL, NULL, { display_next_state(true); });

// general dynamic step
UX_STEP_NOCB(ux_display_general,
             bnnn_paging,
             {
                 .title = g_current_title,
                 .text = g_current_text,
             });

// Lower delimiter step
UX_STEP_INIT(step_lower_delimiter, NULL, NULL, { display_next_state(false); });

// FLOW to display transaction:
// #1 screen: eye icon + "Confirm" + transaction type
// #x screen: Dynamic Screen
// #n-2 screen: memo (if present)
// #n-1 screen: approve button
// #n screen: reject button
UX_FLOW(ux_display_transaction_flow,
        &ux_display_review_step,
        &step_upper_delimiter,
        &ux_display_general,
        &step_lower_delimiter,
        &ux_display_approve_step,
        &ux_display_reject_step);

int ui_display_transaction() {
    if (G_context.req_type != CONFIRM_TRANSACTION || G_context.state != STATE_PARSED) {
        G_context.state = STATE_NONE;
        return io_send_sw(SW_BAD_STATE);
    }

    display_context.offset = 0;

    if (G_context.tx_info.transaction.typeGroup == TYPEGROUP_SOLAR) {
        switch (G_context.tx_info.transaction.type) {
            case BURN: {
                snprintf(g_transaction_name, sizeof(g_transaction_name), "%s", "Burn");

                display_context.f = &burn_type_display;
                break;
            }
            case VOTE: {
                snprintf(
                    g_transaction_name,
                    sizeof(g_transaction_name),
                    "%s%s",
                    G_context.tx_info.transaction.core_asset.Vote.vote_length == 0 ? "Cancel " : "",
                    "Vote");

                display_context.f = &vote_type_display;
                break;
            }
            default:
                return io_send_sw(SW_TX_PARSING_FAIL);
        }
    } else {
        switch (G_context.tx_info.transaction.type) {
            case IPFS: {
                // First screen
                snprintf(g_transaction_name, sizeof(g_transaction_name), "%s", "IPFS");

                display_context.f = &ipfs_type_display;
                break;
            }
            case TRANSFER: {
                snprintf(g_transaction_name, sizeof(g_transaction_name), "%s", "Transfer");

                display_context.f = &transfer_type_display;
                break;
            }
            default:
                return io_send_sw(SW_TX_PARSING_FAIL);
        }
    }

    g_validate_callback = &ui_action_validate_transaction;

    ux_flow_init(0, ux_display_transaction_flow, NULL);

    return 0;
}

// display message
UX_STEP_NOCB(ux_display_message,
             bnnn_paging,
             {
                 .title = "Message",
                 .text = (const char *) &G_context.tx_info.raw_tx + 2,
             });

UX_FLOW(ux_display_message_flow,
        &ux_display_review_step,
        &ux_display_message,
        &ux_display_approve_step,
        &ux_display_reject_step);

int ui_display_message() {
    if (G_context.req_type != CONFIRM_MESSAGE || G_context.state != STATE_PARSED) {
        G_context.state = STATE_NONE;
        return io_send_sw(SW_BAD_STATE);
    }

    // First screen
    snprintf(g_transaction_name, sizeof(g_transaction_name), "%s", "Message");

    g_validate_callback = &ui_action_validate_transaction;

    ux_flow_init(0, ux_display_message_flow, NULL);

    return 0;
}
