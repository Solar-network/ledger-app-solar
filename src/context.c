/*****************************************************************************
 *  Copyright (c) Solar Network <hello@solar.org>
 *
 *  This work is licensed under a Creative Commons Attribution-NoDerivatives
 *  4.0 International License.
 *****************************************************************************/

#include "context.h"

#include <string.h>  // explicit_bzero

#include "os.h"  // PRINTF

#include "globals.h"

void reset_app_context() {
    PRINTF("!!RESET_APP_CONTEXT\n");
    explicit_bzero(&G_context, sizeof(G_context));
    G_context.state = STATE_NONE;
    G_context.req_num = 0;
}
