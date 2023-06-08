/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "ui/transactions/transfer_display.h"

#include <stdio.h>   // snprintf
#include <stdint.h>  // uint*_t

#include "buffer.h"
#include "format.h"
#include "read.h"

#include "address.h"
#include "constants.h"

#include "transaction/transaction_utils.h"
#include "transaction/types.h"
#include "ui/ctx.h"

bool transfer_type_display(transaction_t *tx, char title[], char text[], uint16_t step) {
    // transfers (amount + recipient)
    uint16_t number_recipients = step / 2;
    if (number_recipients < tx->core_asset.Transfer.transfers_length) {
        uint16_t offset = number_recipients * (ADDRESS_HASH_LEN + 8);
        if (step % 2 == 0) {
            // Recipient
            offset += 8;
            snprintf(title,
                     MAX_TITLE_LEN,
                     "%s %d/%d",
                     "Address",
                     number_recipients + 1,
                     tx->core_asset.Transfer.transfers_length);
            base58_encode_address(&tx->core_asset.Transfer.transfers[offset],
                                  ADDRESS_HASH_LEN,
                                  text,
                                  MAX_TEXT_LEN);
        } else {
            // Amount
            snprintf(title,
                     MAX_TITLE_LEN,
                     "%s %d/%d",
                     "Amount",
                     number_recipients + 1,
                     tx->core_asset.Transfer.transfers_length);
            format_amount(text,
                          MAX_TEXT_LEN,
                          read_u64_le(&tx->core_asset.Transfer.transfers[offset], 0),
                          EXPONENT_SMALLEST_UNIT,
                          TICKER_DEFAULT,
                          sizeof(TICKER_DEFAULT));
        }

        return true;
    }

    switch (step - (tx->core_asset.Transfer.transfers_length * 2)) {
        case 0: {
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
