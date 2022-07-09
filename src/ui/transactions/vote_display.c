/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "vote_display.h"
#include "types.h"
#include "constants.h"
#include "../../transaction/types/vote.h"
#include "../../common/buffer.h"
#include "../../common/format.h"
#include "../../common/read.h"
#include "../../common/base58.h"

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
            char title_text[] = "Delegate (%d/%d)";
            if (vote_number >= 9) {
                strcpy(title_text, "Delegate(%d/%d)");
            }
            snprintf(title,
                     MAX_TITLE_LEN,
                     title_text,
                     vote_number + 1,
                     tx->core_asset.Vote.vote_length);

            snprintf(text, MAX_TEXT_LEN, "%.*s", asset.username_length, asset.username);
            return true;
        } else {
            char title_text[] = "Vote %% (%d/%d)";
            if (vote_number >= 9) {
                strcpy(title_text, "Vote %%(%d/%d)");
            }
            snprintf(title,
                     MAX_TITLE_LEN,
                     title_text,
                     vote_number + 1,
                     tx->core_asset.Vote.vote_length);
            format_percentage(text, MAX_TEXT_LEN, asset.percentage, 2);
            return true;
        }
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
