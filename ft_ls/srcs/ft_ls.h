#ifndef FT_LS_H
# define FT_LS_H

# include <dirent.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <time.h>

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

// parser
int find_in_string(char *string, char character);
void parse_command(int argc, char *argv[], t_command *command_structure);

// measures
int ft_strlen(char * string);

// sorting algos
void bubble_sort_output(char **entry_names);

// print data
void print_file_data(struct dirent *pDirent);
void print_file_creation_time(char *path);
void print_file_entry(struct dirent *pDirent, t_command *command_structure);

// memory managers
void free_command_structure(t_command *command_structure);

#endif