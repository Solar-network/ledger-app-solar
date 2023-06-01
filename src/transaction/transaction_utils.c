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

#include "transaction/transaction_utils.h"

#include <stddef.h>   // size_t
#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <string.h>   // strlen
#include <stdio.h>    // snprintf

#include "format.h"

#include "ui/ui_utils.h"

static bool _format_fpu64(char *dst, size_t dst_len, const uint64_t value, uint8_t decimals) {
    char buffer[21] = {0};

    if (!format_u64(buffer, sizeof(buffer), value)) {
        return false;
    }

    size_t digits = strlen(buffer);

    if (digits <= decimals) {
        if (dst_len <= 2 + decimals + 1) {
            return false;
        }
        *dst++ = '0';
        *dst++ = '.';
        for (uint16_t i = 0; i < decimals - digits; i++, dst++) {
            *dst = '0';
        }
        dst_len -= 2 + decimals - digits;
        strncpy(dst, buffer, dst_len);
    } else {
        if (dst_len <= digits + 1) {
            return false;
        }

        const size_t shift = digits - decimals;
        memmove(dst, buffer, shift);
        dst[shift] = '.';
        strncpy(dst + shift + 1, buffer + shift, decimals);
    }

    return true;
}

bool format_amount(char *dst,
                   size_t dst_len,
                   const uint64_t value,
                   uint8_t decimals,
                   const char *ticker,
                   size_t ticker_len) {
    char amount[22] = {0};
    if (dst_len < sizeof(amount) + 5 || ticker_len > 5) {
        return false;
    }
    if (!_format_fpu64(amount, sizeof(amount), value, decimals)) {
        return false;
    }

    unpad_amount(amount, strlen(amount), 2);

    snprintf(dst, dst_len, "%s%c%s", amount, get_ticker_line_break(amount), ticker);
    return true;
}

bool format_percentage(char *dst, size_t dst_len, const uint16_t value, uint8_t decimals) {
    char amount[22] = {0};
    if (dst_len < 9) {
        return false;
    }
    if (!_format_fpu64(amount, sizeof(amount), (const uint64_t) value, decimals)) {
        return false;
    }
    snprintf(dst, dst_len, "%s%%", amount);
    return true;
}

bool transaction_utils_check_ascii(const uint8_t *text, uint64_t text_len, bool allow_new_lines) {
    for (uint64_t i = 0; i < text_len; i++) {
        bool lf = text[i] == 0x0A;
        bool crlf = text[i] == 0x0D && i + 1 < text_len && text[i + 1] == 0x0A;
        if (!((allow_new_lines && (lf || crlf)) || (text[i] >= 0x20 && text[i] <= 0x7E))) {
            return false;
        }
    }
    return true;
}
