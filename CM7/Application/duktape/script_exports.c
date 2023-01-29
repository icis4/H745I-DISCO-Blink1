/*
 * script_exports.c
 *
 *  Created on: 28 May 2020
 *      Author: Ivaylo Ilchev iic@melexis.com
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

#include "duktape.h"
#include "script_exports.h"

uint32_t duktype_timeout_start = 0;
uint32_t duktype_timeout = 10000; /* 10 sec */

#define JS_TIMEOUT_CHECK() \
do {\
    if (duktype_timeout_check(NULL)) {\
        duk_push_error_object(ctx, DUK_ERR_RANGE_ERROR, "execution timeout");\
        duk_throw_raw(ctx);\
        return DUK_RET_ERROR;\
    }\
} while(0);

duk_bool_t duktype_timeout_check(void *udata)
{
    /* Handle ticks overflow */
    if (osKernelSysTick() - duktype_timeout_start < duktype_timeout)
        return 0;
    return 1;
}

duk_ret_t duk_native_print(duk_context *ctx)
{
    JS_TIMEOUT_CHECK();

    FILE* fout = stdout;

    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    fputs(duk_safe_to_string(ctx, -1), fout);
    fputc('\n', fout);
    fflush(fout);
    return 0;
}

duk_ret_t duk_native_alert(duk_context *ctx)
{
    JS_TIMEOUT_CHECK();

    duk_push_string(ctx, " ");
    duk_insert(ctx, 0);
    duk_join(ctx, duk_get_top(ctx) - 1);
    puts(duk_safe_to_string(ctx, -1));
    fflush(stdout);
    return 0;
}

duk_ret_t duk_native_sleep(duk_context *ctx)
{
    uint32_t ms = duk_get_uint(ctx, 0);

    if (duktype_timeout_check(NULL) ||
            osKernelSysTick() + ms - duktype_timeout_start >= duktype_timeout) {
        duk_push_error_object(ctx, DUK_ERR_RANGE_ERROR, "execution timeout");
        duk_throw_raw(ctx);
        return DUK_RET_ERROR;
    }

    osDelay(ms);

    return 0;
}

duk_ret_t duk_native_time(duk_context *ctx)
{
    JS_TIMEOUT_CHECK();

    duk_push_uint(ctx, osKernelSysTick());
    return 1;
}

