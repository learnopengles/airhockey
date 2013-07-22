#include "platform_log.h"
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>

#define ANDROID_LOG_VPRINT(priority)	va_list arg_ptr; \
										va_start(arg_ptr, fmt); \
										__android_log_vprint(priority, tag, fmt, arg_ptr); \
										va_end(arg_ptr);

void _debug_log_v(const char *tag, const char *fmt, ...) {
	ANDROID_LOG_VPRINT(ANDROID_LOG_VERBOSE);
}

void _debug_log_d(const char *tag, const char *fmt, ...) {
	ANDROID_LOG_VPRINT(ANDROID_LOG_DEBUG);
}

void _debug_log_w(const char *tag, const char *fmt, ...) {
	ANDROID_LOG_VPRINT(ANDROID_LOG_WARN);
}

void _debug_log_e(const char *tag, const char *fmt, ...) {
	ANDROID_LOG_VPRINT(ANDROID_LOG_ERROR);
}
