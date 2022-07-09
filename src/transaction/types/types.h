/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#pragma once

#include <transaction/types/multi_signature_registration.h>
#include <transaction/types/ipfs.h>
#include <transaction/types/transfer.h>
#include <transaction/types/htlc_lock.h>
#include <transaction/types/htlc_claim.h>
#include <transaction/types/htlc_refund.h>
#include <transaction/types/burn.h>
#include <transaction/types/vote.h>

typedef union transaction_asset_t {
    multisignature_transaction_asset_t Multisignature;
    ipfs_transaction_asset_t Ipfs;
    transfer_transaction_asset_t Transfer;
    htlc_lock_transaction_asset_t Htlc_lock;
    htlc_claim_transaction_asset_t Htlc_claim;
    htlc_refund_transaction_asset_t Htlc_refund;
    burn_transaction_asset_t Burn;
    vote_transaction_asset_t Vote;
} transaction_asset_t;
