/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *
 *****************************************************************************
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *
 *  This software also incorporates work covered by the following copyright
 *  and permission notice:
 *
 *   Ledger App Boilerplate.
 *   (c) 2023 Ledger SAS.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include "ui/ui_utils.h"

#include <stddef.h>   // size_t
#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <string.h>   // strlen

#include "bolos_target.h"

#if defined(TARGET_NANOS)  // Estimate amount/fee line pixel count for the Nano S.

// copied from:
// https://github.com/LedgerHQ/nanos-secure-sdk/blob/master/lib_ux/include/ux_layout_paging_compute.h#L8
#define PIXEL_PER_LINE 114

// extern from:
// https://github.com/LedgerHQ/nanos-secure-sdk/blob/master/lib_ux/src/ux_layout_paging_compute.c#L20-#L117
extern const char nanos_characters_width[96];

// adapted from:
// https://github.com/LedgerHQ/nanos-secure-sdk/blob/master/lib_ux/src/ux_layout_paging_compute.c#L147-#L175
static uint32_t calculate_pixels(char *text, size_t text_length) {
    char current_char;
    uint32_t line_width = 0;

    while (text_length--) {
        current_char = *text;
        line_width += (nanos_characters_width[current_char - 0x20] >> 0x04) & 0x0F;
        text++;
    }

    return line_width;
}

#define GET_TICKER_LINE_BREAK(num_str) \
    (((calculate_pixels((num_str), strlen(num_str))) >= ((PIXEL_PER_LINE) - (1))) ? ('\n') : (' '))

#else  // if NOT Nano S, Only return the space character.

#define GET_TICKER_LINE_BREAK(num_str) ((void) (num_str), (' '))

#endif

void unpad_amount(char *amount, size_t len, size_t padding) {
    char *ptr = amount + len - 1;
    while (*ptr == '0' && *(ptr - padding) != '.') {
        *ptr-- = 0;
    }
}

char get_ticker_line_break(char *num_str) {
    return GET_TICKER_LINE_BREAK(num_str);
}
