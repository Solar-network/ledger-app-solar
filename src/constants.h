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

/**
 * Instruction class of the Solar application.
 */
#define CLA 0xE0

/**
 * Length of APPNAME variable in the Makefile.
 */
#define APPNAME_LEN (sizeof(APPNAME) - 1)

/**
 * Maximum length of MAJOR_VERSION || MINOR_VERSION || PATCH_VERSION.
 */
#define APPVERSION_LEN 3

/**
 * Maximum length of application name.
 */
#define MAX_APPNAME_LEN 64

/**
 * Maximum transaction length (bytes).
 */
#define MAX_TRANSACTION_LEN 1600

/**
 * Exponent used to convert SXP unit (N SXP = N * 10^8).
 */
#define EXPONENT_SMALLEST_UNIT 8

/**
 * Length of 32 bytes hash.
 */
#define HASH_32_LEN 32

/**
 * Length of 64 bytes hash.
 */
#define HASH_64_LEN 64

/**
 * Length of the address hash ([network/version(1), pubKeyHash(20)]).
 */
#define ADDRESS_HASH_LEN 21

/**
 * String length of a Solar address.
 */
#define ADDRESS_LEN 34

/**
 * Solar Mainnet Network Byte
 */
#define NETWORK_SOLAR_MAINNET 0x3F  // 63

/**
 * Solar Mainnet Network Byte
 */
#define NETWORK_SOLAR_TESTNET 0x1E  // 30

/**
 * Length of compressed public key.
 */
#define PUBLIC_KEY_LEN 33

/**
 * Length of schnorr signature.
 */
#define SIG_SCHNORR_LEN 64

/**
 * Max length for memo.
 */
#define MAX_MEMO_LEN 255

/**
 * Max length for the hash on HTLC transactions.
 */
#define MAX_HTLC_HASH_LEN 192

/**
 * The default ticker name used for displaying amounts.
 */
#define TICKER_DEFAULT "SXP"
