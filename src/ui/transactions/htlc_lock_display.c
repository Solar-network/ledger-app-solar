/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include <stdio.h>
#include "htlc_lock_display.h"
#include "types.h"
#include "constants.h"
#include "address.h"
#include "../../common/buffer.h"
#include "../../common/format.h"

bool htlc_lock_type_display(transaction_t *tx, char title[], char text[], uint16_t step) {
    switch (step) {
        case 0: {
            // RecipientId
            snprintf(title, MAX_TITLE_LEN, "%s", "Recipient");
            base58_encode_address(tx->core_asset.Htlc_lock.recipientId,
                                  ADDRESS_HASH_LEN,
                                  text,
                                  MAX_TEXT_LEN);
            break;
        }
        case 1: {
            // Amount
            snprintf(title, MAX_TITLE_LEN, "%s", "Amount");
            format_amount(text,
                          MAX_TEXT_LEN,
                          tx->core_asset.Htlc_lock.amount,
                          EXPONENT_SMALLEST_UNIT,
                          TICKER_DEFAULT,
                          sizeof(TICKER_DEFAULT));
            break;
        }
        case 2: {
            // Secret hash
            snprintf(title, MAX_TITLE_LEN, "%s", "Secret Hash");
            format_hex(tx->core_asset.Htlc_lock.secret_hash,
                       tx->core_asset.Htlc_lock.secret_hash_length,
                       text,
                       MAX_TEXT_LEN);
            break;
        }
        case 3: {
            // Expiration
            bool isTime = tx->core_asset.Htlc_lock.expiration_type == 1;
            snprintf(title, MAX_TITLE_LEN, "%s", isTime ? "Expire Time" : "Expire Height");
            snprintf(text, MAX_TEXT_LEN, "%d", tx->core_asset.Htlc_lock.expiration_value);
            break;
        }
        case 4: {
            // Fee
            snprintf(title, MAX_TITLE_LEN, "%s", "Fee");
            format_amount(text,
                          MAX_TEXT_LEN,
                          tx->fee,
                          EXPONENT_SMALLEST_UNIT,
                          TICKER_DEFAULT,
                          sizeof(TICKER_DEFAULT));
            break;
        }
        default: {
            return false;
        }
    }

    return true;
}
