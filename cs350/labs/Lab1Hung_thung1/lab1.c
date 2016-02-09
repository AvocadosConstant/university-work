#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <getopt.h>
#include <time.h>
#include "mytimer.h"

int intcmp(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

void print_usage_string() {
    char c;
    FILE *file = fopen("res/usage.txt", "rt");
    while((c = getc(file)) != EOF) {
        printf("%c",c);   
    }
    fclose(file);
}

void sort_mode(FILE *istream, FILE *ostream, FILE *cstream, int num, int min, int max) {
    int next_int = 0;
    fscanf(istream, "%d\n", &next_int);  //  size of input data set

    if(next_int < num) {
        fprintf(stderr, "ERROR: Requested to sort %d values, but input file has only %d values.\n\n", num, next_int);
        return;
    }

    int *arr;
    arr = (int *) malloc(num * sizeof(int));

    int i = 0;
    for(i = 0; i < num; i++) {         //  reading in values
        fscanf(istream, "%d", &next_int);
        if(next_int < min || next_int > max) {
            fprintf(stderr, "Input contains value outside of bounds: %d to %d.\n\n", min, max);
            return;
        }
        arr[i] = next_int;
    }

    qsort(arr, num, sizeof(int), intcmp);
    
    //  printing sorted values
    for(i = 0; i < num; i++) fprintf(ostream, "%d\n", arr[i]);
    
    //  printing userid counts
	char *user = getenv("USER");
    int count;
    int j;
    for (i = 0; i < strlen(user); i++){
        count = 0;
        for(j = 0; j < num; j++) if(arr[j] == user[i]) count++;
        fprintf(cstream, "%c\t%d\t%d\n", user[i], user[i], count);
    }
    free(arr);
}

void gen_mode(FILE *ostream, int num, int min, int max) {
    fprintf(ostream, "%d\n", num);  //  print number of nums to generate
    int i;
    for(i = 0; i < num; i++) {
        fprintf(ostream, "%d\n", rand() % (max - min + 1) + min);
    }
}

int main(int argc, char **argv) {
    static int mode = 0;    //  sort mode = 0; generate mode = 1;

    static int num_int = 100;
    static int min_int = 1;
    static int max_int = 255;

    FILE *istream = stdin;
    FILE *ostream = stdout;
    FILE *cstream = stdout;

    srand(time(NULL));

    extern char *optarg;
    int opt = 0;
	while((opt = getopt(argc, argv, "ugn:m:M:s:i:o:c:")) != -1) {
		switch(opt) {
			case 'u':{      //  prints the usage string
                print_usage_string();
                return 0;
            }
            case 'g':
                mode = 1;   //  set to generate mode
                break;
			case 'n':
                if((num_int = atoi(optarg)) < 0 || num_int > 1000000) {
                    fprintf(stderr, "Number of integers out of bounds.\n");   
                    return 0;
                }
				break;
			case 'm':
                if((min_int = atoi(optarg)) < 1 || min_int > 255) {
                    fprintf(stderr, "Minimum integer out of bounds.\n");   
                    return 0;
                }
				break;
			case 'M':
                if((max_int = atoi(optarg)) < 1 || max_int > 255) {
                    fprintf(stderr, "Maximum integer out of bounds.\n");   
                    return 0;
                }
				break;
			case 's':
                srand(atoi(optarg));
				break;
			case 'i':
                if((istream = fopen(optarg, "rt")) == NULL) {
                    fprintf(stderr, "Cannot open input file.\n");
                    return 0;
                }
				break;
			case 'o':
                if((ostream = fopen(optarg, "w")) == NULL) {
                    fprintf(stderr, "Cannot open output file.\n");
                    return 0;
                }
				break;
			case 'c':
                if((cstream = fopen(optarg, "w")) == NULL) {
                    fprintf(stderr, "Cannot open count output file.\n");
                    return 0;
                }
				break;
			default:
                print_usage_string();
				return 0;		
		}
	}

    //  input range checking
    if(min_int > max_int) {
        fprintf(stderr, "ERROR: Minimum integer (%d) is greater than maximum integer (%d).\n", min_int, max_int);
    }

    //  sort mode
    if(mode == 0) {     
        start_timer();
        sort_mode(istream, ostream, cstream, num_int, min_int, max_int);
        stop_timer();
        print_elapsed();
    }

    //  generate mode
    else gen_mode(ostream, num_int, min_int, max_int);

    fclose(ostream);
    fclose(istream);
    fclose(cstream);

    return 0;
}

