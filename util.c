/* See LICENSE file for copyright and license details. */
#include <err.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util.h"

const char *
bprintf(const char *fmt, ...)
{
	va_list ap;
	size_t len;

	va_start(ap, fmt);
	len = vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
	va_end(ap);

	if (len >= sizeof(buf))
		buf[sizeof(buf)-1] = '\0';

	return buf;
}

int
pscanf(const char *path, const char *fmt, ...)
{
	FILE *fp;
	va_list ap;
	int n;

	if (!(fp = fopen(path, "r"))) {
		warn("fopen %s: %s\n", path, strerror(errno));
		return -1;
	}
	va_start(ap, fmt);
	n = vfscanf(fp, fmt, ap);
	va_end(ap);
	fclose(fp);

	return (n == EOF) ? -1 : n;
}

const char *
perctobar(const char *perc, const char *fmt)
{
	char temp[sizeof(fmt)-1] = "";
	const char *bars[] = { "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█" };
	int i;
	int j=0;
	for (i=0; fmt[i]!= '\0'; i++) {
		if (&fmt[i] == strstr(fmt, "VBAR")) {
			if (perc == NULL || atoi(perc)>100) {
				strcat(temp, "X");
				j=j+1;
			} else {
				strcat(temp, (bars[(7 * atoi(perc)) / 100]));
				j=j+3;
			}
		} else if (&fmt[i] < strstr(fmt, "VBAR") || &fmt[i-3] > strstr(fmt, "VBAR")) {
			temp[j] = fmt[i];
			j=j+1;
		}
	}
	return bprintf("%s",temp);
}
