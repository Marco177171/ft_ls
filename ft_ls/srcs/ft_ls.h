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
	int		multiple_folders;
	char	*flags;
	char	**folder_list;
	int		folder_count;
} t_command;

typedef struct s_output {
	char	permission_table[11];
	char	**file_entries;
	char	*owner_name;
	int		subfolder_amount;
} t_output;

typedef struct s_entry {
	char *name;
	struct stat fileStat;
} t_entry;

// parser
int find_in_string(char *string, char character);
void parse_command(int argc, char *argv[], t_command *command_structure);

// measures
int ft_strlen(char *string);

// sorting algos
void sort_entries(t_entry *entries);

// print data
void print_file_data(t_entry *entry);
void print_file_creation_time(t_entry *entry);
void print_file_entry(t_entry *entry, t_command *command_structure);

// memory managers
void free_command_structure(t_command *command_structure);

#endif