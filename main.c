#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>

#include<nerror.h>

int result_count = 0;

int search_word_in_dir(char *search_word, char *path);

int main(int argc, char **argv)
{
	if (argc <= 1) {
		fprintf(stderr, "findf: missing arguments\nTRY:\n\tfind --help\n");
		return 1;
	}
	else if (argc == 2) {
		if ((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0)) {
			fprintf(stderr, "No help for you just yet...\n");
			return 0;
		}
	}
	else if (argc >= 3) {
		fprintf(stderr, "findf: too many arguments\nTRY:\n\tfind --help\n");
		return 1;
	}
	
	search_word_in_dir(argv[1], ".");

	printf("\n%d results returned\n", result_count);

	return 0;
}

int search_word_in_dir(char *search_word, char *path)
{
	DIR *current_dir;
	struct dirent *dir_reader;
	struct stat stat_buffer;

	current_dir = opendir(path);
	FAIL_IF_R(current_dir == NULL, 1);

	int chdir_test;
	chdir_test = chdir(path);
	FAIL_IF_R(chdir_test != 0, 1);

	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	FAIL_IF_R(cwd == NULL, 1);
	
	int search_size = strlen(search_word);

	for (int counter = 0; counter != strlen(path); counter++) {
		if (path[counter] == search_word[0]) {
			for (int i = 0; i <= search_size ; i++) {
				if (path[counter+i] == search_word[i]) {
					if (i == search_size - 1) {
						printf("%s\n", cwd);
						result_count++;
					}
				}
				else if (path[counter+i] == '\0') {
					break;
				}
				else {
					break;
				}
			}
		}
	}

	while ((dir_reader = readdir(current_dir)) != NULL) {
		lstat(dir_reader->d_name, &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode)) {
			if ((strcmp(".", dir_reader->d_name) != 0) && (strcmp("..", dir_reader->d_name) != 0)) {
				search_word_in_dir(search_word, dir_reader->d_name);
			}
		}
		else {
			for (int counter = 0; counter <= strlen(dir_reader->d_name); counter++) {
				if (dir_reader->d_name[counter] == search_word[0]) {
					for (int i = 0; i <= search_size; i++) {
						if (dir_reader->d_name[counter+i] == search_word[i]) {
							if (i == search_size - 1) {
								getcwd(cwd, sizeof(cwd));
								strcat(cwd, "/");
								strcat(cwd, dir_reader->d_name);
								printf("%s <FILE>\n", cwd);
								result_count++;
							}
						}
						else if (dir_reader->d_name[counter+i] == '\0') {
							break;
						}
						else {
							break;
						}
					}
				}
			}
		}
	}
	
	chdir_test = chdir("..");
	FAIL_IF_R(chdir_test != 0, 1);

	closedir(current_dir);

	return 0;
}
