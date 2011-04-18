/*
 * String utilities
 *
 * Copyright (c) 2009-2011 Centro Svizzero di Calcolo Scientifico (CSCS)
 * Licensed under the GPLv2.
 */
#define _ISOC99_SOURCE		/* for LLONG_{MIN,MAX} */
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>

/**
 * atou64 - Convert string into u64
 * Returns 1 if ok, < 0 on error.
 */
int atou64(const char *str, uint64_t *val)
{
	char *endptr;

	errno = 0;				/* strtol() manpage */
	*val  = strtoull(str, &endptr, 0);
	if ((errno == ERANGE && *val == ULLONG_MAX) ||
	    (errno != 0 && *val == 0)	||	/* other error */
	    endptr == str		||	/* no digits */
	    *endptr != '\0')			/* junk at end */
		return -1;
	return 1;
}

int atou32(const char *str, uint32_t *val)
{
	uint64_t tmp;

	if (!atou64(str, &tmp) || tmp > 0xFFFFffffUL)
		return -1;
	*val = tmp;
	return 1;
}

/*
 * POSIX says time_t can be integer or floating type.
 * On x86_32 it is an u32, on x86_64 it is an u64 type.
 */
int atotime_t(const char *str, time_t *val)
{
	uint64_t tmp;

	if (!atou64(str, &tmp))
		return -1;
	*val = tmp;
	return 1;
}

/**
 * parse_version_string  -  extract release numbers from version string
 * @vs:    version string satisfying ^D*\d+.(\d*|\d+.\d*)
 * @major: major number to extract
 * @minor: minor number to extract (defaults to 0 if not given)
 * @micro: micro number to extract (defaults to 0 if not given)
 * Returns 0 if ok (@vs satisfies above regexp), else -1 to indicate error.
 */
int parse_version_string(const char *vs, unsigned *major,
			 unsigned *minor, unsigned *micro)
{
	if (vs == NULL)
		return -1;
	while (*vs && !isdigit(*vs))
		vs++;
	for (*major = 0; isdigit(*vs); vs++)
		*major = *major * 10 + *vs - '0';
	if (*vs == '\0' || *vs != '.' || (*++vs && !isdigit(*vs)))
		return -1;
	for (*minor = *micro = 0; isdigit(*vs); vs++)
		*minor = *minor * 10 + *vs - '0';
	if (*vs == '.')
		while (isdigit(*++vs))
			*micro = *micro * 10 + *vs - '0';
	return 0;
}
