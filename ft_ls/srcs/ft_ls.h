#ifndef FT_LS_H
# define FT_LS_H

# include <dirent.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <time.h>
# include <pwd.h>
# include <grp.h>

typedef struct s_command {
	char	*flags;
	char	**folder_list;
	int		folder_count;
	int		multiple_folders;
} t_command;

typedef struct s_entry {
	char		*name;
	struct stat	*fileStat;
} t_entry;

// command managers
void init_command(int argc, char **argv, t_command *command_struct);
void free_command(t_command *command_structure);

// utilities
int find_in_string(char *string, char character);
int ft_strlen(char *str);
int ft_strcmp(char *first, char *second);

// sorting algos
void sort(t_command *command, t_entry *entries, int entries_amount);
void sort_entries(t_entry *entries, int entries_amount);
void sort_entries_by_time(t_entry *entries, int entries_amount);

// printers
void print_file_data(t_entry entry);
void print_file_creation_time(t_entry entry);
void print_file_entry(t_entry *entry, t_command *command_structure, int entries_amount);

#endif