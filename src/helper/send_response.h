#pragma once

#include "os.h"

#include "macros.h"

/**
 * Length of chain code.
 */
#define CHAINCODE_LEN (MEMBER_SIZE(pubkey_ctx_t, chain_code))

/**
 * Helper to send APDU response with public key and chain code.
 *
 * response = PUBKEY_LEN (1) ||
 *            G_context.pk_info.public_key (PUBKEY_LEN) ||
 *            CHAINCODE_LEN (1) ||
 *            G_context.pk_info.chain_code (CHAINCODE_LEN)
 *
 * @return zero or positive integer if success, -1 otherwise.
 *
 */
int helper_send_response_pubkey(void);

/**
 * Helper to send APDU response with public key and chain code.
 *
 * response = ADDRESS_LEN(1) | ADDRESS(34)
 *
 * @return zero or positive integer if success, -1 otherwise.
 *
 */
int helper_send_response_address(void);

/**
 * Helper to send APDU response with signature.
 *
 * response = G_context.tx_info.signature
 *
 * @return zero or positive integer if success, -1 otherwise.
 *
 */
int helper_send_response_sig(void);
