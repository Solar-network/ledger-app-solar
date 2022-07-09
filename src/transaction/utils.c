/*****************************************************************************
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *
 *  This software also incorporates work covered by the following copyright
 *  and permission notice:
 *
 *   Ledger App Boilerplate.
 *   (c) 2020 Ledger SAS.
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

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool
#include <string.h>   // memmove

#include "types.h"

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
