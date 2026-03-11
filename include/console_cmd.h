#ifndef CONSOLE_CMD_H_
#define CONSOLE_CMD_H_

#define COMMONLIB_REMOVE_PREFIX
#include <commonlib.h>

typedef struct Console_cmd Console_cmd;
typedef struct Console_cmds Console_cmds;
typedef void (*Console_cmd_func)(String_array, const char *);

struct Console_cmd {
	const char *name;
	const char *desc;

	Console_cmd_func func;
};

struct Console_cmds {
	Console_cmd *items;
	size_t count;
	size_t capacity;
};

// Funcs
void help_f(String_array args, const char *rest);
void exit_f(String_array args, const char *rest);

#endif // CONSOLE_CMD_H_
