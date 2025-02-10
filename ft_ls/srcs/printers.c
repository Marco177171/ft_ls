#include "ft_ls.h"

void print_file_data(t_entry entry) {
	write(1, (S_ISDIR(entry.fileStat->st_mode))  ? "d" : "-", 1);
	write(1, (entry.fileStat->st_mode & S_IRUSR) ? "r" : "-", 1);
	write(1, (entry.fileStat->st_mode & S_IWUSR) ? "w" : "-", 1);
	write(1, (entry.fileStat->st_mode & S_IXUSR) ? "x" : "-", 1);
	write(1, (entry.fileStat->st_mode & S_IRGRP) ? "r" : "-", 1);
	write(1, (entry.fileStat->st_mode & S_IWGRP) ? "w" : "-", 1);
	write(1, (entry.fileStat->st_mode & S_IXGRP) ? "x" : "-", 1);
	write(1, (entry.fileStat->st_mode & S_IROTH) ? "r" : "-", 1);
	write(1, (entry.fileStat->st_mode & S_IWOTH) ? "w" : "-", 1);
	write(1, (entry.fileStat->st_mode & S_IXOTH) ? "x" : "-", 1);
	write(1, " ", 1);
}

void print_file_creation_time(t_entry entry) {
	char * time = ctime(&entry.fileStat->st_mtime);
	time[16] = '\0';
	write(1, time + 4, 12);
	write(1, " ", 1);
}

void print_file_entry(t_entry *entry, t_command *command_structure, int entries_amount) {
	int index = 0;

	if (find_in_string(command_structure->flags, 'r')) {
		index = entries_amount - 1;

		while (index >= 0) {
			if (find_in_string(command_structure->flags, 'l') == 1) {
				struct passwd *pwd = getpwuid(entry[index].fileStat->st_uid);
				struct group *grp = getgrgid(entry[index].fileStat->st_uid);
				print_file_data(entry[index]);
				if (find_in_string(command_structure->flags, 'g') != 1)
					write(1, strcat(pwd->pw_name, " "), ft_strlen(pwd->pw_name) + 1);
				write(1, strcat(grp->gr_name, " "), ft_strlen(grp->gr_name) + 1);
				print_file_creation_time(entry[index]);
			}
			write(1, entry[index].name, ft_strlen(entry[index].name));
			write(1, find_in_string(command_structure->flags, 'l') == 1 ? "\n" : " ", 1);
			index--;
		}
	}
	else {
		while (index < entries_amount) {
			if (find_in_string(command_structure->flags, 'l') == 1) {
				struct passwd *pwd = getpwuid(entry[index].fileStat->st_uid);
				struct group *grp = getgrgid(entry[index].fileStat->st_uid);
				print_file_data(entry[index]);
				if (find_in_string(command_structure->flags, 'g') != 1)
					write(1, strcat(pwd->pw_name, " "), ft_strlen(pwd->pw_name) + 1);
				write(1, strcat(grp->gr_name, " "), ft_strlen(grp->gr_name) + 1);
				print_file_creation_time(entry[index]);
			}
			write(1, entry[index].name, ft_strlen(entry[index].name));
			write(1, find_in_string(command_structure->flags, 'l') == 1 ? "\n" : " ", 1);
			index++;
		}
	}
}
