#pragma once

/**
 * String length of a Solar address.
 */
#define ADDRESS_LEN 34

/**
 * Length of the address hash ([network/version(1), pubKeyHash(20)]).
 */
#define ADDRESS_HASH_LEN 21

/**
 * Length of APPNAME variable in the Makefile.
 */
#define APPNAME_LEN (sizeof(APPNAME) - 1)

/**
 * Maximum length of application name.
 */
#define APPNAME_MAX_LEN 64

/**
 * Maximum length of MAJOR_VERSION || MINOR_VERSION || PATCH_VERSION.
 */
#define APPVERSION_LEN 3

/**
 * Instruction class of the Solar application.
 */
#define CLA 0xE0

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
 * The default ticker name used for displaying amounts.
 */
#define TICKER_DEFAULT "SXP"

/**
 * Maximum transaction length (bytes).
 */
#define TRANSACTION_MAX_LEN 1600
