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

#pragma once

#include "types.h"
#include "../common/buffer.h"

/**
 * Deserialise raw transaction in structure.
 *
 * @param[in, out] buf
 *   Pointer to buffer with serialised transaction.
 * @param[out]     tx
 *   Pointer to transaction structure.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */
parser_status_e transaction_deserialise(buffer_t *buf, transaction_t *tx);

/**
 * Deserialise common part of the raw transaction.
 *
 * @param[in, out] buf
 *   Pointer to buffer with serialised transaction.
 * @param[out]     tx
 *   Pointer to transaction structure.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */
parser_status_e transaction_deserialise_common(buffer_t *buf, transaction_t *tx);

/**
 * Deserialise transaction specific asset of the raw transaction.
 *
 * @param[in, out] buf
 *   Pointer to buffer with serialised transaction.
 * @param[out]     tx
 *   Pointer to transaction assets structure.
 * @param[in]     type
 *   Transaction type.
 * @param[in]     typeGroup
 *   Transaction typeGroup.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */
parser_status_e transaction_deserialise_core_asset(buffer_t *buf,
                                                   transaction_asset_t *tx,
                                                   uint16_t type,
                                                   uint32_t typeGroup);

/**
 * Deserialise message in structure.
 *
 * @param[in, out] buf
 *   Pointer to buffer with message.
 * @param[out]     tx
 *   Pointer to transaction structure.
 *
 * @return PARSING_OK if success, error status otherwise.
 *
 */
parser_status_e message_deserialise(buffer_t *buf, transaction_t *tx);
