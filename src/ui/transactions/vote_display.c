/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "ui/transactions/vote_display.h"

#include <stdint.h>  // uint*_t
#include <stdio.h>   // snprintf
#include <string.h>

#include "base58.h"
#include "buffer.h"
#include "format.h"
#include "read.h"

#include "constants.h"
#include "types.h"

#include "transaction/transaction_utils.h"
#include "transaction/types/vote.h"
#include "ui/ctx.h"

bool vote_type_display(transaction_t *tx, char title[], char text[], uint16_t step) {
    uint16_t vote_count = step / 2;

    // Check if we are still in the voting phase
    if (vote_count < tx->core_asset.Vote.vote_length) {
        vote_deserialised_t asset = {0};
        vote_search(tx->core_asset.Vote.votes, vote_count, tx->core_asset.Vote.vote_length, &asset);

        if (step % 2 == 0) {
            // Prepare the vote[n] screen
            snprintf(title,
                     MAX_TITLE_LEN,
                     "Vote (%d/%d)",
                     vote_count + 1,
                     tx->core_asset.Vote.vote_length);
            snprintf(text, MAX_TEXT_LEN, "%.*s", asset.username_length, asset.username);
        } else {
            // Prepare the vote percentage[n] screen
            snprintf(title,
                     MAX_TITLE_LEN,
                     "Vote %% (%d/%d)",
                     vote_count + 1,
                     tx->core_asset.Vote.vote_length);
            format_percentage(text, MAX_TEXT_LEN, asset.percentage, 2);
        }

        return true;
    }

    // Prepare the fee screen
    if (step - (tx->core_asset.Vote.vote_length * 2) == 0) {
        snprintf(title, MAX_TITLE_LEN, "Fee");
        format_amount(text,
                      MAX_TEXT_LEN,
                      tx->fee,
                      EXPONENT_SMALLEST_UNIT,
                      TICKER_DEFAULT,
                      sizeof(TICKER_DEFAULT));
        return true;
    }

    return false;
}

bool vote_search(uint8_t *tx, uint8_t vote_number, uint8_t max_votes, vote_deserialised_t *asset) {
    if (vote_number >= max_votes) {
        return false;
    }

    uint8_t *ptr = tx;
    for (uint8_t i = 0; i < vote_number; i++) {
        uint8_t username_length = *ptr;
        ptr += username_length + 3;
    }
    asset->username_length = *ptr++;
    asset->username = ptr;
    ptr += asset->username_length;
    asset->percentage = read_u16_le(tx, ptr - tx);

    return true;
}
