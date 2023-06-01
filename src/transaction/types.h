#pragma once

#include <stdint.h>  // uint*_t

#include "transaction/types/types.h"

#define STARTING_BYTE         0xFF
#define TRANSACTION_VERSION_3 0x03

typedef enum { TYPEGROUP_CORE = 1, TYPEGROUP_SOLAR = 2 } transaction_typegroup;

typedef enum { IPFS = 5, TRANSFER = 6 } transaction_type_core;
typedef enum { BURN = 0, VOTE = 2 } transaction_type_solar;

typedef struct {
    // Message
    uint16_t message_length;  /// Message length (4 bytes)
    uint8_t *message;         /// message
    // Transaction
    uint32_t typeGroup;         /// typeGroup (4 bytes)
    uint16_t type;              /// type (2 bytes)
    uint8_t *sender_publickey;  /// sender public key (33 bytes)
    uint64_t fee;               /// fee (8 bytes)
    uint8_t memo_len;           /// length of memo (1 byte)
    uint8_t *memo;              /// memo (MAX length 255)

    transaction_asset_t core_asset;  /// transaction specific assets (variable length and structure)
} transaction_t;
