#ifndef CONFIG_H_
#define CONFIG_H_

#include <engine.h>
#include <console_cmd.h>

#define CMDS_CAPACITY (1024)

extern int WIDTH;
extern int HEIGHT;
extern Font font;
extern Console console;
extern bool console_shown;
extern const char *cmd_names[CMDS_CAPACITY];
extern size_t cmd_names_count;
extern Console_cmds cmds;

void init_globals(void);

#endif // CONFIG_H_
