#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>

#include<nerror.h>

int search_word_in_dir(char *search_word, char *path);

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

	search_word_in_dir(argv[1], ".");

	return 0;
}

int search_word_in_dir(char *search_word, char *path)
{
	DIR *current_dir;
	struct dirent *dir_reader;
	struct stat stat_buffer;

	current_dir = opendir(path);
	FAIL_IF_R_M(current_dir == NULL, 1, stderr, "Error opening file\n");

	int chdir_test;
	chdir_test = chdir(path);
	FAIL_IF_R_M(chdir_test != 0, 1, stderr, "Error changing path");
	
	printf("Directory of %s:\n", path);

	while ((dir_reader = readdir(current_dir)) != NULL) {
		stat(dir_reader->d_name, &stat_buffer);
		if (S_ISDIR(stat_buffer.st_mode)) {
			if ((strcmp(".", dir_reader->d_name) != 0) && (strcmp("..", dir_reader->d_name) != 0)) {
				search_word_in_dir(search_word, dir_reader->d_name);
			}
		}
		else {
			printf("%s <FILE>\n", dir_reader->d_name);
		}
	}
	
	fputc('\n', stdout);

	chdir_test = chdir("..");
	FAIL_IF_R_M(chdir_test != 0, 1, stderr, "Error changing path");

	closedir(current_dir);

	return 0;
}
