#if !defined(__BAZYLUM_H)
#define __BAZYLUM_H

#include "list.h"

struct window_stat {
	char *window_name;
	int window_time;
};

struct bazylum_stat {
	struct listnode *window_stat;

	unsigned int avg_time;
	unsigned int max_time;
};

#endif
