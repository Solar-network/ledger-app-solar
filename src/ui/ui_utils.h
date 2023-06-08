#pragma once

#include <stddef.h>  // size_t

/**
 * Remove trailing zeros up to the decimal + padding.
 *
 * @param[in] amount
 *   Pointer to amount string.
 * @param[in]  len
 *   Length of amount string.
 * @param[in]  padding
 *   Length of zero padding to keep.
 *
 */
void unpad_amount(char *amount, size_t len, size_t padding);

/**
 * Determine if a new page should be used for ticker display.
 *
 * (e.g., avoid `123456789012345678 / 9.00 SXP` being misread as `9.00 SXP`)
 *
 * @param[in] num_str
 *   Pointer to formatted number string.
 *
 * @return new line character if needed for Nano S, otherwise a space character.
 *
 */
char get_ticker_line_break(char *num_str);
