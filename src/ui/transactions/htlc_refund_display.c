/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include <stdio.h>
#include "htlc_refund_display.h"
#include "types.h"
#include "constants.h"
#include "../../common/buffer.h"
#include "../../common/format.h"
#include "../../common/base58.h"

bool htlc_refund_type_display(transaction_t *tx, char title[], char text[], uint16_t step) {
    switch (step) {
        case 0: {
            // Lock Transaction Id
            snprintf(title, MAX_TITLE_LEN, "%s", "Lock ID");
            format_hex(tx->core_asset.Htlc_refund.lockId, HASH_32_LEN, text, MAX_TEXT_LEN);
            break;
        }
        case 1: {
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
