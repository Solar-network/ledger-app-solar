/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "ui/ctx.h"

#include "transaction/types.h"

bool context_get_next(ctx_t *ctx, transaction_t *tx, char title[], char text[]) {
    bool result = ctx->f(tx, title, text, ctx->offset);
    ctx->offset++;
    return result;
}

bool context_get_previous(ctx_t *ctx, transaction_t *tx, char title[], char text[]) {
    if (ctx->offset < 2) {
        ctx->offset = 0;
        return false;
    }
    ctx->offset--;
    return ctx->f(tx, title, text, ctx->offset - 1);
}
