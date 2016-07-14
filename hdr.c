#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <wchar.h>

#include "blaze822.h"

static size_t l;
static char *hdr;

void
header(char *file)
{
	struct message *msg;

	msg = blaze822(file);
	if (!msg)
		return;

	char *v = blaze822_hdr_(msg, hdr, l);
	if (v)
		printf("%s\n", v);
}

static void
printhdr(char *hdr)
{
	int uc = 1;

	while (*hdr && *hdr != ':') {
		putc(uc ? toupper(*hdr) : *hdr, stdout);
		uc = (*hdr == '-');
		hdr++;
	}
	fputs(hdr, stdout);
	fputc('\n', stdout);
}

void
headerall(char *file)
{
	struct message *msg;

	msg = blaze822(file);
	if (!msg)
		return;

	char *h = 0;
	while (h = blaze822_next_header(msg, h))
		printhdr(h);
}

int
main(int argc, char *argv[])
{
	if (argv[1] && argv[1][0] == '-') {
		l = strlen(argv[1])+1;
		hdr = malloc(l);
		hdr[0] = 0;
		char *s = hdr+1;
		char *t = argv[1]+1;
		while (*t)
			*s++ = tolower(*t++);
		*s = ':';

		blaze822_loop(argc-2, argv+2, header);
	} else {
		blaze822_loop(argc-1, argv+1, headerall);
	}
	
	return 0;
}
