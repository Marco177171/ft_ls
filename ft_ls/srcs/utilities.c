#include "ft_ls.h"

int ft_strlen(char *str) {
	int index = 0;
	while (str[index])
		index++;
	return index;
}

int find_in_string(char *string, char character) {
	int index = 0;

	while (string[index] != '\0') {
		if (string[index] == character)
			return (1);
		index++;
	}
	return (0);
}

int ft_strcmp(char *first, char *second) {
	int i = 0, j = 0;

	while (first[i]) {
		while (second[j]) {
			if (first[i] != second[j])
				return (first[i] - second[j]);
			j++;
		}
		i++;
	}
	return 0;
}