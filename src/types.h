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

#include <stdbool.h>  // bool
#include <stddef.h>   // size_t
#include <stdint.h>   // uint*_t

#include "constants.h"
#include "transaction/types.h"
#include "common/bip32.h"

/**
 * Enumeration for the status of IO.
 */
typedef enum {
    READY,     /// ready for new event
    RECEIVED,  /// data received
    WAITING    /// waiting
} io_state_e;

/**
 * Enumeration with expected INS of APDU commands.
 */
typedef enum {
    GET_APP_NAME = 0xa1,    /// name of the application
    GET_VERSION = 0xa2,     /// version of the application
    GET_PUBLIC_KEY = 0xb1,  /// public key of corresponding BIP32 path
    GET_ADDRESS = 0xb2,     /// address of corresponding BIP32 path
    SIGN_MESSAGE = 0xc1,    /// sign message with BIP32 path
    SIGN_TX = 0xc2          /// sign transaction with BIP32 path
} command_e;

/**
 * Structure with fields of APDU command.
 */
typedef struct {
    uint8_t cla;    /// Instruction class
    command_e ins;  /// Instruction code
    uint8_t p1;     /// Instruction parameter 1
    uint8_t p2;     /// Instruction parameter 2
    uint8_t lc;     /// Lenght of command data
    uint8_t *data;  /// Command data
} command_t;

/**
 * Enumeration with parsing state.
 */
typedef enum {
    STATE_NONE,     /// No state
    STATE_PARSED,   /// Transaction data parsed
    STATE_APPROVED  /// Transaction data approved
} state_e;

/**
 * Enumeration with user request type.
 */
typedef enum {
    CONFIRM_PUBLICKEY,    /// confirm public key
    CONFIRM_ADDRESS,      /// confirm address
    CONFIRM_TRANSACTION,  /// confirm transaction information
    CONFIRM_MESSAGE       /// confirm message information
} request_type_e;

/**
 * Structure for public key context information.
 */
typedef struct {
    uint8_t raw_public_key[33];  /// compressed public key
    uint8_t chain_code[32];      /// for public key derivation
    bool use_chaincode;          /// whether the chain code should be used
} pubkey_ctx_t;

/**
 * Structure for transaction information context.
 */
typedef struct {
    uint8_t raw_tx[MAX_TRANSACTION_LEN];  /// raw transaction serialised
    size_t raw_tx_len;                    /// length of raw transaction
    transaction_t transaction;            /// structured transaction
    uint8_t m_hash[32];                   /// message hash digest
    uint8_t signature[SIG_SCHNORR_LEN];   /// transaction signature
} transaction_ctx_t;

/**
 * Structure for global context.
 */
typedef struct {
    state_e state;    /// state of the context
    uint8_t req_num;  /// current request number
    union {
        pubkey_ctx_t pk_info;       /// public key context
        transaction_ctx_t tx_info;  /// transaction context
    };
    request_type_e req_type;              /// user request
    uint32_t bip32_path[MAX_BIP32_PATH];  /// BIP32 path
    uint8_t bip32_path_len;               /// lenght of BIP32 path
    uint8_t network;                      /// network byte
} global_ctx_t;
