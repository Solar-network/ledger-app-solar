#pragma once

#include <stddef.h>   // size_t
#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool

/**
 * Format 64-bit unsigned integer as string with decimals and ticker.
 *
 * @param[out] dst
 *   Pointer to output string.
 * @param[in]  dst_len
 *   Length of output string.
 * @param[in]  value
 *   64-bit unsigned integer to format.
 * @param[in]  decimals
 *   Number of digits after decimal separator.
 * @param[in]  ticker
 *   The token's ticker name to be displayed.
 * @param[in]  ticker_len
 *   The length of the ticker name (not including the null-terminator).
 *
 * @return true if success, false otherwise.
 *
 */
bool format_amount(char *dst,
                   size_t dst_len,
                   const uint64_t value,
                   uint8_t decimals,
                   const char *ticker,
                   size_t ticker_len);

/**
 * Format 16-bit unsigned integer as string with decimals and percentage sign.
 *
 * @param[out] dst
 *   Pointer to output string.
 * @param[in]  dst_len
 *   Length of output string.
 * @param[in]  value
 *   16-bit unsigned integer to format.
 * @param[in]  decimals
 *   Number of digits after decimal separator.
 *
 * @return true if success, false otherwise.
 *
 */
bool format_percentage(char *dst, size_t dst_len, const uint16_t value, uint8_t decimals);

/**
 * Check if input is encoded using ASCII characters.
 *
 * @param[in] text
 *   Pointer to input byte buffer.
 * @param[in] text_len
 *   Length of input byte buffer.
 * @param[in] allow_new_lines
 *   Allow ASCII text to contain new lines.
 *
 * @return true if success, false otherwise.
 *
 */
bool transaction_utils_check_ascii(const uint8_t *text, uint64_t text_len, bool allow_new_lines);
