/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "ui/transactions/ipfs_display.h"

#include <stdbool.h>  // bool
#include <stdint.h>   // uint*_t
#include <stdio.h>    // snprintf

#include "base58.h"
#include "buffer.h"
#include "format.h"

#include "constants.h"

#include "transaction/transaction_utils.h"
#include "transaction/types.h"
#include "ui/ctx.h"

bool ipfs_type_display(transaction_t *tx, char title[], char text[], uint16_t step) {
    switch (step) {
        case 0: {
            // IPFS hash
            snprintf(title, MAX_TITLE_LEN, "Content ID");
            char encoded[100] = {0};
            int base58_length = base58_encode(tx->core_asset.Ipfs.ipfs,
                                              tx->core_asset.Ipfs.ipfs_length,
                                              encoded,
                                              100);
            snprintf(text, MAX_TEXT_LEN, "%.*s", base58_length, encoded);
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
