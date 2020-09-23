#ifndef TRACE_H_
#define TRACE_H_

/* --------------------------------------------------------------------------
 * HEADERS
 * -------------------------------------------------------------------------- */
//Modifié par Marilys L
#include "uart.h"
#define TRACE_NONE 0
#define TRACE_FATAL 1
#define TRACE_ERROR 2
#define TRACE_WARNING 3
#define TRACE_INFO 4
#define TRACE_DEBUG 5
#define TRACE_DEV 6

#define TRACE_LEVEL TRACE_INFO



/* --------------------------------------------------------------------------
 * PUBLIC MACROS
 * -------------------------------------------------------------------------- */

#if (TRACE_LEVEL > TRACE_NONE)
#define trace_fatal(module,fmt, ...) ({ PRINT(rs_ctx,"[ %s ] ",module); PRINT(rs_ctx,fmt, ##__VA_ARGS__); PRINT(rs_ctx,"\n");})
#else
#define trace_fatal(module,fmt, ...) do {} while (0)
#endif

#if (TRACE_LEVEL > TRACE_FATAL)
#define trace_error(module,fmt, ...) ({ PRINT(rs_ctx,"FATAL [ %s ] ",module); PRINT(rs_ctx,fmt, ##__VA_ARGS__); PRINT(rs_ctx,"\n");})
#else
#define trace_error(module,fmt, ...) do {} while (0)
#endif

#if (TRACE_LEVEL > TRACE_ERROR)
#define trace_warning(module,fmt, ...) ({ PRINT(rs_ctx,"ERROR [ %s ] ",module); PRINT(rs_ctx,fmt, ##__VA_ARGS__); PRINT(rs_ctx,"\n");})
#else
#define trace_warning(module,fmt, ...) do {} while (0)
#endif

#if (TRACE_LEVEL > TRACE_WARNING)
#define trace_info(module,fmt, ...) ({ PRINT(rs_ctx,"WARNING [ %s ] ",module); PRINT(rs_ctx,fmt, ##__VA_ARGS__); PRINT(rs_ctx,"\n");})
#else
#define trace_info(module,fmt, ...) do {} while (0)
#endif

#if (TRACE_LEVEL > TRACE_INFO)
#define trace_debug(module,fmt, ...) ({ PRINT(rs_ctx,"[ %s ] ",module); PRINT(rs_ctx,fmt, ##__VA_ARGS__); PRINT(rs_ctx,"\n");})
#define trace_marker() printf(VT100_COLOR_CYAN "[%s:%d]\n" VT100_COLOR_END, __FUNCTION__, __LINE__);
#else
#define trace_debug(module,fmt, ...) do {} while (0)
#define trace_marker() do {} while (0)
#endif

#if (TRACE_LEVEL > TRACE_DEBUG)
#define trace_dev(module,fmt, ...) ({ PRINT(rs_ctx,"[ %s ] ",module); PRINT(rs_ctx,fmt, ##__VA_ARGS__); PRINT(rs_ctx,"\n");})
#else
#define trace_dev(module,fmt, ...) do {} while (0)
#endif

#endif /* !TRACE_H_ */
