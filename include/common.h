#ifndef COMMON_H_
#define COMMON_H_

#define COMMONLIB_REMOVE_PREFIX
#include <commonlib.h>

typedef struct {
	int *items;
	size_t count;
	size_t capacity;
} Ids;

// TODO: Move these to commonlib/engine
// Debug:
bool str_starts_with(const char *str, const char *suffix);
Ids match_command(const char *command, const char **commands, size_t commands_count);


#endif // COMMON_H_
