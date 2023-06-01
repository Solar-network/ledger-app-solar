/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#pragma once

#include "transaction/types/ipfs.h"
#include "transaction/types/transfer.h"
#include "transaction/types/burn.h"
#include "transaction/types/vote.h"

typedef union transaction_asset_t {
    ipfs_transaction_asset_t Ipfs;
    transfer_transaction_asset_t Transfer;
    burn_transaction_asset_t Burn;
    vote_transaction_asset_t Vote;
} transaction_asset_t;
