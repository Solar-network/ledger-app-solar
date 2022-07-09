/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include <stdio.h>
#include "multi_signature_registration_display.h"
#include "types.h"
#include "constants.h"
#include "../../common/buffer.h"
#include "../../common/format.h"

bool multisignature_type_display(transaction_t *tx, char title[], char text[], uint16_t step) {
    // Public keys
    if (step < tx->core_asset.Multisignature.pkey_length) {
        snprintf(title,
                 MAX_TITLE_LEN,
                 "%s %d/%d",
                 "PublicKey",
                 step + 1,
                 tx->core_asset.Multisignature.pkey_length);
        format_hex(&tx->core_asset.Multisignature.pkeys[PUBLIC_KEY_LEN * step],
                   PUBLIC_KEY_LEN,
                   text,
                   MAX_TEXT_LEN);
        return true;
    }

    switch (step - tx->core_asset.Multisignature.pkey_length) {
        case 0: {
            // Minimum
            snprintf(title, MAX_TITLE_LEN, "%s", "Minimum");
            snprintf(text, MAX_TEXT_LEN, "%d", tx->core_asset.Multisignature.min);
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
