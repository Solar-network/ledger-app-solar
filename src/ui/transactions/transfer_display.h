/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

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
bool transfer_type_display(transaction_t *tx, char title[], char text[], uint16_t step);
