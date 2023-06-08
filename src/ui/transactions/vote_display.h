#pragma once

#include <stdbool.h>  // bool
#include <stdint.h>   // uint*_t

#include "transaction/types.h"

/**
 * Produce content for specific transaction.
 *
 * @param[in] tx
 *   Pointer to transaction structure.
 * @param[out] title
 *   Title output.
 * @param[out] text
 *   Text output.
 * @param[in] step
 *   current step to display.
 *
 * @return true if there is content, false otherwise.
 *
 */
bool vote_type_display(transaction_t *tx, char title[], char text[], uint16_t step);

/**
 * Iterate the vote asset to find the current one.
 *
 * @param[in] tx
 *   Pointer to votes.
 * @param[in] vote_number
 *   Current vote number in the list to return.
 * @param[in] max_votes
 *   Length of the vote list.
 * @param[out] asset
 *   The structure of the desidered vote is returned.
 *
 * @return true if there is content, false otherwise.
 *
 */
bool vote_search(uint8_t *tx, uint8_t vote_number, uint8_t max_votes, vote_deserialised_t *asset);
