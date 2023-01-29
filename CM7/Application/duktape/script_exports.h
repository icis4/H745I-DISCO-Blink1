/*
 * script_exports.h
 *
 *  Created on: 28 May 2020
 *      Author: Ivaylo Ilchev iic@melexis.com
 */

#ifndef DUKTAPE_SCRIPT_EXPORTS_H_
#define DUKTAPE_SCRIPT_EXPORTS_H_

#include "duktape/duktape.h"

#define DUKTYPE_INDEX 3

#define EXP_PROP_FUNCTION(CTX, FNAME, C_FNAME, NPARAMS) \
		duk_push_c_function(CTX, C_FNAME, NPARAMS); \
		duk_put_prop_string(CTX, -2, FNAME)

#define EXP_GLOBAL_FUNCTION(CTX, FNAME, C_FNAME, NPARAMS) \
		duk_push_c_function(CTX, C_FNAME, NPARAMS); \
		duk_put_global_string(CTX, FNAME)

#ifdef  __cplusplus
extern "C"
{
#endif


extern uint32_t duktype_timeout_start;
extern uint32_t duktype_timeout;

extern duk_ret_t duk_native_alert(duk_context *ctx);
extern duk_ret_t duk_native_print(duk_context *ctx);
extern duk_ret_t duk_native_sleep(duk_context *ctx);
extern duk_ret_t duk_native_time(duk_context *ctx);

#ifdef  __cplusplus
}
#endif

#endif /* DUKTAPE_SCRIPT_EXPORTS_H_ */
