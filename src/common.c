#include <common.h>

bool str_starts_with(const char *str, const char *suffix) {
    if (str == NULL) return false;
    while (*str != 0 && *suffix != 0) {
        if (*suffix++ != *str++) {
            return false;
        }
    }
    return true;
}

Ids match_command(const char *command, const char **commands, size_t commands_count) {
    Ids matched_command_ids = {0};
    size_t command_len = strlen(command);
    char command_lower[1024] = {0};

    //
    size_t max = sizeof(command_lower) - 1;
    size_t copy_len = command_len > max ? max : command_len;
    for (size_t i=0;i<copy_len;++i) command_lower[i]=tolower((unsigned char)command[i]);
    command_lower[copy_len]=0;
    if (command_len > max) command_len = copy_len; // or treat as truncated
    //

    for (int i = 0; i < commands_count; ++i) {
        const char *c = commands[i];
        size_t c_len = strlen(c);
        if (str_starts_with(c, command_lower)) {
            if ((command_len > c_len && command_lower[c_len] == ' ')
              || command_len <= c_len) {
                darr_append(matched_command_ids, i);
            }
        }
    }

    return matched_command_ids;
}
