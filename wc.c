#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void print_usage(char *msg);

int *get_counts(char *filename);

void print_counts(int *show, int *count, char *name);

void print_counts(int *show, int *count, char *name)
{
	if (show[0] == 1)
	{
		printf("%8d ", count[0]);
	}
        if (show[1] == 1)
        {
                printf("%8d ", count[1]);
        }
        if (show[2] == 1)
        {
                printf("%8d ", count[2]);
        }

	printf("%s\n", name);
}

void print_usage(char *msg)
{
	if (msg != NULL)
	{
		printf("%s\n", msg);
	}
	printf("\nUsage: wc [-l] [-w] [-c] [FILES...]\n");
	printf("where:\n");
	printf("       -l    prints the number of lines\n");
	printf("       -w    prints the number of words\n");
	printf("       -c    prints the number of characters\n");
	printf("       FILES if no files are given, then read\n");
	printf("      	     from standard input\n");
	exit(0);
}

int *get_counts(char *filename)
{	
	if (filename == NULL)
	{
		return NULL;
	}
	int *count_array = (int *)malloc(3 * sizeof(int));
	int fd;
	int size = 1;
	int in_whitespace = 1;
	if (filename[0] != '\0')
	{
		fd = open(filename, O_RDONLY);
		if (fd == -1)
		{
			perror(filename);
			return NULL;
		}
	}
	else
	{
		fd = 0;
	}
	char buf[1];
	size = read(fd, buf, sizeof(char));
	if (size < 0)
        {
		perror("read");
                exit(1);
        }
	while (size != 0) 
	{
		if (strcmp(buf, "\n") == 0)
		{
			count_array[0] += 1;
		}

		count_array[2] += 1;
                
		if (isspace(buf[0]) && in_whitespace == 0)
                {
                        count_array[1] += 1;
                        in_whitespace = 1;
                }
                if (isgraph(buf[0]) && in_whitespace == 1)
                {
                        in_whitespace = 0;
                }		
		size = read(fd, buf, sizeof(char));
                if (size < 0)
                {
                        perror("read");
                        exit(1);
                }	
	}
	if (in_whitespace == 0)
	{
		count_array[1] += 1;		
	}
	if (filename[0] != '\0')
	{
		close(fd);
	}
	return count_array;
}

int main(int argc, char **argv)
{
	int show[3] = {0, 0, 0};
	char *name;
	int *count_array;
	int totals[3] = {0, 0, 0};
	int file_count = 0;
	int no_args = 1;
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-l") == 0)
		{
			show[0] = 1;
			no_args = 0;
		}
		else if (strcmp(argv[i], "-w") == 0)
                {
			show[1] = 1;
			no_args = 0;
                }
		else if (strcmp(argv[i], "-c") == 0)
                {
			show[2] = 1;
			no_args = 0;
                }
		
		else
		{
	                if (argv[i][0] == '-')
			{
				char *msg = "invalid argument";
				print_usage(msg);
			}
			if (no_args == 1)
                	{
                        	for (int i = 0; i  < 3; ++i)
                        	{
                                	show[i] = 1;
                        	}
                	}
	        	name = argv[i];
        		count_array = get_counts(name);
			if (count_array == NULL)
			{
				if (i == argc-1)
				{
					exit(1);
				}
				continue;
			}
			totals[0] += count_array[0];
			totals[1] += count_array[1];
                        totals[2] += count_array[2];
			print_counts(show, count_array, name);
			file_count += 1;
		}
	}
	if (file_count > 1)
	{
		print_counts(show, totals, "total");
	}
	if (file_count == 0)
	{
		name = "";
                count_array = get_counts(name);
	        if (no_args == 1)
		{
			for (int i = 0; i  < 3; ++i)
                	{
                		show[i] = 1;
                	}
		}
		print_counts(show, count_array, name);
	}
	free(count_array);
	return 0;
}
