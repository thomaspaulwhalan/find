#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>

#include<nerror.h>

int search_word_in_dir(char *search_word, char *path, int cwd_size);

int main(int argc, char **argv)
{
	if (argc <= 1) {
		fprintf(stderr, "find: missing arguments\nTRY:\n\tfind --help\n");
		return 1;
	}
	else if (argc == 2) {
		if ((strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "-h") == 0)) {
			fprintf(stderr, "No help for you just yet...\n");
			return 0;
		}
	}
	else if (argc >= 3) {
		fprintf(stderr, "find: too many arguments\nTRY:\n\tfind --help\n");
		return 1;
	}

	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	FAIL_IF_R_M(cwd == NULL, 1, stderr, "Error getting cwd()\n");
	int cwd_size = strlen(cwd);
	
	search_word_in_dir(argv[1], ".", cwd_size);

	return 0;
}

int search_word_in_dir(char *search_word, char *path, int cwd_size)
{
	DIR *current_dir;
	struct dirent *dir_reader;
	struct stat stat_buffer;

	current_dir = opendir(path);
	FAIL_IF_R_M(current_dir == NULL, 1, stderr, "Error opening file\n");

	int chdir_test;
	chdir_test = chdir(path);
	FAIL_IF_R_M(chdir_test != 0, 1, stderr, "Error changing path");

	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	FAIL_IF_R_M(cwd == NULL, 1, stderr, "Error getting cwd()\n");
	
	int search_size = strlen(search_word);

	for (int counter = cwd_size; counter != strlen(cwd); counter++) {
		if (cwd[counter] == search_word[0]) {
			for (int i = 0; i <= search_size ; i++) {
				if (cwd[counter+i] == search_word[i]) {
					if (i == search_size - 1) {
						printf("%s\n", cwd);
					}
				}
				else if (cwd[counter+i] == '\0') {
					break;
				}
			}
		}
	}

	while ((dir_reader = readdir(current_dir)) != NULL) {
		stat(dir_reader->d_name, &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode)) {
			if ((strcmp(".", dir_reader->d_name) != 0) && (strcmp("..", dir_reader->d_name) != 0)) {
				search_word_in_dir(search_word, dir_reader->d_name, cwd_size);
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
	FAIL_IF_R_M(chdir_test != 0, 1, stderr, "Error changing path");

	closedir(current_dir);

	return 0;
}
