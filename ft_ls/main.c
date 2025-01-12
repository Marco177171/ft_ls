// allowed functions

// ◦ write
// ◦ opendir
// ◦ readdir
// ◦ closedir
// ◦ stat
// ◦ lstat
// ◦ getpwuid
// ◦ getgrgid
// ◦ listxattr
// ◦ getxattr
// ◦ time
// ◦ ctime
// ◦ readlink
// ◦ malloc
// ◦ free
// ◦ perror
// ◦ strerror
// ◦ exit

// errors:
// ls: cannot access '-': No such file or directory

// The stat(2) system call returns a struct stat that contains a st_mode member. This is the mode bits that ls -l displays.

// On my system, the man 2 stat page says:

//    The following flags are defined for the st_mode field:

//    S_IFMT     0170000   bitmask for the file type bitfields
//    S_IFSOCK   0140000   socket
//    S_IFLNK    0120000   symbolic link
//    S_IFREG    0100000   regular file
//    S_IFBLK    0060000   block device
//    S_IFDIR    0040000   directory
//    S_IFCHR    0020000   character device
//    S_IFIFO    0010000   FIFO
//    S_ISUID    0004000   set UID bit
//    S_ISGID    0002000   set-group-ID bit (see below)
//    S_ISVTX    0001000   sticky bit (see below)
//    S_IRWXU    00700     mask for file owner permissions
//    S_IRUSR    00400     owner has read permission
//    S_IWUSR    00200     owner has write permission
//    S_IXUSR    00100     owner has execute permission
//    S_IRWXG    00070     mask for group permissions
//    S_IRGRP    00040     group has read permission
//    S_IWGRP    00020     group has write permission
//    S_IXGRP    00010     group has execute permission
//    S_IRWXO    00007     mask for permissions for others (not in group)
//    S_IROTH    00004     others have read permission
//    S_IWOTH    00002     others have write permission
//    S_IXOTH    00001     others have execute permission

// FLAGS to follow:
// -l, -R, -a, -r and -t
// bouns: -u, -f, -g, -d

// find given documents -> eg: ls -la Documents/*.pdf -> only show pdfs

// todo: recursive lists of subdirectories, order and reverse order on output print
// sort by time insted of ascii

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

typedef struct ls_command {
	int		multiple_folders;
	char	*flags;
	char	**folder_list;
	int		folder_count;
} ls_command;

typedef struct ls_output {
	char	permission_table[11];
	char	*owner_name;
	int		subfolder_amount;
} ls_output;

void ls_error(const char *error_string) {
	perror(error_string);
	exit (1);
}

int find_in_string(char *string, char character) {
	int index = 0;

	// printf("[DEBUG] : finding %c among flags. Flags: %s\n", character, string);
	while (string[index] != '\0') {
		// printf("[DEBUG] : searching at index %d: %c\n", index, string[index]);
		if (string[index] == character) {
			// printf("[DEBUG] : Found! Returning...\n");
			return (1);
		}
		index++;
	}
	// printf("[DEBUG] : Not found! Returning...\n");
	return (0);
}

void print_file_data(struct dirent *pDirent) {
	struct stat fileStat;

	stat(pDirent->d_name, &fileStat);
	// if (S_ISDIR(fileStat.st_mode))
	// 	printf("[DEBUG] : %s is a directory!\n", pDirent->d_name);
	printf((S_ISDIR(fileStat.st_mode))  ? "d" : "-");
	printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
	printf(" ");
}

void print_file_creation_time(char *path) {
    struct stat attr;

    stat(path, &attr);
    printf("%s ", ctime(&attr.st_mtime));
}

void print_file_entry(struct dirent *pDirent, ls_command *command_structure) {
	if (find_in_string(command_structure->flags, 'l') == 1) {
		print_file_data(pDirent);
		print_file_creation_time(pDirent->d_name);
	}
	printf("%s", pDirent->d_name);
	// if (find_in_string(command_structure->flags, 'l') == 1)
	// 	printf("\n");
	
	printf(find_in_string(command_structure->flags, 'l') == 1  ? "\n" : "\t");
	// printf("It's d_ino is %ld\t", pDirent->d_ino);
	// printf("It's d_off is %ld\t", pDirent->d_off);
	// printf("It's d_type is %d\n", pDirent->d_type);
}

int ft_strlen(char * string) {
	int index = 0;

	while (string[index] != '\0')
		index++;
	return (index);
}

void free_command_structure(ls_command *command_structure) {
	int index = 0;

	if (command_structure->flags)
		free(command_structure->flags);
	if (command_structure->folder_list)
		while (command_structure->folder_list[index++])
            free(command_structure->folder_list[index]);
}

void parse_command(char *argv[], ls_command *command_structure) {
	int index = 1;
	int flag_index = 0;
	int flag_string_index = 0;

	command_structure->multiple_folders = 0;
	command_structure->flags = malloc(sizeof(char*) * 256);
	command_structure->folder_count = 0;
	if (!command_structure->flags) {
		perror("[ERROR] : malloc failed");
		exit(EXIT_FAILURE);
	}
	command_structure->flags[0] = '\0'; // Initialize the string
	command_structure->folder_list = malloc(sizeof(char**) * 256);
	if (!command_structure->folder_list) {
		perror("[ERROR] : malloc on folder's list failed");
        free_command_structure(command_structure);
		exit(EXIT_FAILURE);
	}
	while (argv[index] != NULL) {
		// printf("[DEBUG] : checking args: %s\n", argv[index]);
		if (argv[index][0] == '-') {
			// printf("[DEBUG] : new flag_list found\n");
			flag_index = 1;
			while (argv[index][flag_index]) {
				command_structure->flags[flag_string_index] = argv[index][flag_index];
				flag_string_index++;
				flag_index++;
			}
		}
		else {
			if (command_structure->folder_count > 0)
				command_structure->multiple_folders = 1;
			command_structure->folder_list[command_structure->folder_count] = strdup(argv[index]);
			// printf("[DEBUG] : list of folders to write updated: %s\n", command_structure->folder_list[command_structure->folder_count]);
			if (!command_structure->folder_list[command_structure->folder_count]) {
                // perror("[ERROR] : strdup failed");
        		free_command_structure(command_structure);
                exit(EXIT_FAILURE);
            }
			command_structure->folder_count++;
		}
		index++;
	}
    if (command_structure->folder_count == 0) {
        command_structure->folder_list[0] = strdup(".");
        if (!command_structure->folder_list[0]) {
            perror("[ERROR] : strdup failed");
            free(command_structure->flags);
            free(command_structure->folder_list);
            exit(EXIT_FAILURE);
        }
        command_structure->folder_count++;
    }
	// command_structure->folder_count = 0;
	command_structure->flags[flag_string_index] = '\0';
	// printf("[DEBUG] : list of flags: %s\n", command_structure->flags);
}

int main(int argc, char *argv[]) {
	DIR				*directory;
	struct dirent	*pDirent;
	struct stat		fileStat;
	ls_command		command_structure;
	char			buffer[1024];
	char			*abspath;
	int				folder_index = 0;
	time_t			current_time;

	time(&current_time);
	printf("%s", ctime(&current_time));

	parse_command(argv, &command_structure);
	while (command_structure.folder_list[folder_index]) {
		// printf("[DEBUG] : folder at %d : %s\n", folder_index, command_structure.folder_list[folder_index]);
		folder_index++;
	}
	folder_index = 0;
	while (command_structure.folder_list[folder_index]) {
		// printf("[DEBUG] : cycle in %s\n", command_structure.folder_list[folder_index]);
		abspath = realpath(command_structure.folder_list[folder_index], buffer);
		if (!abspath) {
			// printf("[DEBUG] : could not find absolute path.\n");
			folder_index++;
			continue ;
		}
		directory = opendir(abspath);
		if (folder_index > 0 && command_structure.multiple_folders == 1)
			printf("\n");
		if (command_structure.multiple_folders == 1)
			printf("%s:\n", command_structure.folder_list[folder_index]);
		if (!directory) {
			// printf("[DEBUG] : ft_ls: cannot access '%s': No such file or directory\n", abspath);
			continue ;
		}
		while (pDirent = readdir(directory)) {
			// printf("[DEBUG] : cycling dirent %s\n", pDirent->d_name);
			if (pDirent->d_name[0] == '.' && find_in_string(command_structure.flags, 'a') != 1)
				continue ;
			stat(pDirent->d_name, &fileStat);
			// if (S_ISDIR(fileStat.st_mode) && find_in_string(command_structure.flags, 'R') == 1)
			// 	printf("[DEBUG] : found a subdirectory to list recursively: %s\n", pDirent->d_name);
			print_file_entry(pDirent, &command_structure);
		}
		if (find_in_string(command_structure.flags, 'l') != 1)
			printf("\n");
		folder_index++;
	}
	free_command_structure(&command_structure);
	closedir(directory);
	return (0);
}