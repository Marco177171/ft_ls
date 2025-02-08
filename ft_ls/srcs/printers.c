#include "ft_ls.h"

void print_file_data(t_entry *entry) {
	write(1, (S_ISDIR(entry->fileStat.st_mode))  ? "d" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IRUSR) ? "r" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IWUSR) ? "w" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IXUSR) ? "x" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IRGRP) ? "r" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IWGRP) ? "w" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IXGRP) ? "x" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IROTH) ? "r" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IWOTH) ? "w" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IXOTH) ? "x" : "-", 1);
	write(1, " ", 1);
}

void print_file_creation_time(t_entry *entry) {
	write(1, ctime(&entry->fileStat.st_ctime), 19);
	write(1, " ", 1);
}

void print_file_entry(t_entry *entry, t_command *command_structure) {
	if (find_in_string(command_structure->flags, 'l') == 1) {
		struct passwd *pwd = getpwuid(entry->fileStat.st_uid);
		struct group *grp = getgrgid(entry->fileStat.st_gid);
		print_file_data(entry);
		if (find_in_string(command_structure->flags, 'g') != 1)
			write(1, strcat(pwd->pw_name, " "), strlen(pwd->pw_name) + 1);
		write(1, strcat(grp->gr_name, " "), strlen(grp->gr_name) + 1);
		print_file_creation_time(entry);
	}
	write(1, entry->name, strlen(entry->name));
	write(1, find_in_string(command_structure->flags, 'l') == 1 ? "\n" : " ", 1);
}