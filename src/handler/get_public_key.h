#pragma once

#include <stdbool.h>  // bool

#include "buffer.h"

/**
 * Handler for GET_PUBLIC_KEY command. If it successfully parses BIP32 path,
 * derive public key/chain code and send APDU response.
 *
 * @see G_context.bip32_path, G_context.pk_info.raw_public_key and
 *      G_context.pk_info.chain_code.
 *
 * @param[in,out] cdata
 *   Command data with BIP32 path.
 * @param[in]     user_approval
 *   0 for not displaying on screen, 1 otherwise.
 * @param[in]     use_chain_code
 *   0 for not requesting the chain code, 1 otherwise.
 *
 * @return zero or positive integer if success, negative integer otherwise.
 *
 */
int handler_get_public_key(buffer_t *cdata, bool user_approval, bool use_chain_code);
