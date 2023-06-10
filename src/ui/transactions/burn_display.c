/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "ui/transactions/burn_display.h"

#include <stdio.h>   // snprintf
#include <stdint.h>  // uint*_t

#include "base58.h"
#include "buffer.h"

#include "constants.h"

#include "transaction/transaction_utils.h"
#include "transaction/types.h"
#include "ui/ctx.h"

bool burn_type_display(transaction_t *tx, char title[], char text[], uint16_t step) {
    switch (step) {
        case 0: {
            // Amount
            snprintf(title, MAX_TITLE_LEN, "Amount");
            format_amount(text,
                          MAX_TEXT_LEN,
                          tx->core_asset.Burn.amount,
                          EXPONENT_SMALLEST_UNIT,
                          TICKER_DEFAULT,
                          sizeof(TICKER_DEFAULT));
            break;
        }
        case 1: {
            // Fee
            snprintf(title, MAX_TITLE_LEN, "Fee");
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
