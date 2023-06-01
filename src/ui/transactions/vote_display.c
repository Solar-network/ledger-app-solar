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
    // Votes
    uint16_t vote_number = step / 2;
    if (vote_number < tx->core_asset.Vote.vote_length) {
        vote_deserialised_t asset = {0};
        vote_search(tx->core_asset.Vote.votes,
                    vote_number,
                    tx->core_asset.Vote.vote_length,
                    &asset);
        if (step % 2 == 0) {
            snprintf(title,
                     MAX_TITLE_LEN,
                     vote_number >= 9 ? "Block Producer(%d/%d)" : "Block Producer (%d/%d)",
                     vote_number + 1,
                     tx->core_asset.Vote.vote_length);

            snprintf(text, MAX_TEXT_LEN, "%.*s", asset.username_length, asset.username);
        } else {
            snprintf(title,
                     MAX_TITLE_LEN,
                     vote_number >= 9 ? "Vote %%(%d/%d)" : "Vote %% (%d/%d)",
                     vote_number + 1,
                     tx->core_asset.Vote.vote_length);

            format_percentage(text, MAX_TEXT_LEN, asset.percentage, 2);
        }
        return true;
    }

    switch (step - (tx->core_asset.Vote.vote_length * 2)) {
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

bool vote_search(uint8_t *tx, uint8_t vote_number, uint8_t max_votes, vote_deserialised_t *asset) {
    if (vote_number >= max_votes) {
        return false;
    }
    uint16_t offset = 0;
    for (uint8_t i = 0; i < vote_number; i++) {
        uint8_t username_length = *(tx + offset);
        offset += username_length + 3;
    }
    asset->username_length = *(tx + offset);
    offset += 1;
    asset->username = tx + offset;
    offset += asset->username_length;
    asset->percentage = read_u16_le(tx, offset);
    return true;
}
