/*
  Copyright by Stefan Kurtz (C) 1999-2003
  =====================================
  You may use, copy and distribute this file freely as long as you
   - do not change the file,
   - leave this copyright notice in the file,
   - do not make any profit with the distribution of this file
   - give credit where credit is due
  You are not allowed to copy or distribute this file otherwise
  The commercial usage and distribution of this file is prohibited
  Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
*/

/*
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz.
 * For full source control tree, see https://github.com/lsund/wotd
 */


#include "wotd.h"


Bool (*searchfun) (Uchar *, Uchar *);


///////////////////////////////////////////////////////////////////////////////
// Functions


static void print_statistics(FILE *fp, int trials)
{
    fprintf(fp, "%lu ", (Showuint) trials);
    fprintf(fp, "%.2f\n", getruntime()/(double) ITER);
}


///////////////////////////////////////////////////////////////////////////////
// Public API


void run_patterns(const char *path, int npatterns, char ***o_patterns)
{

    inittree();
    initclock();

    int noccurs     = 0;
    char **patterns = *o_patterns;
    FILE *fp        = open_append(path);

    for(int j = 0; j < npatterns; j++) {

        char *current_pattern = patterns[j];
        Uint patternlen = strlen(current_pattern);

        Bool exists = try_search_pattern(current_pattern, patternlen);

        if (exists) {
            fprintf(fp, "%s\n", patterns[j]);
            noccurs++;
        }
    }

    printf("noccurs: %d\n", noccurs);
    printtime();

    fclose(fp);
}


void run_benchmark(const char *path, Uint trials, Uint minpat, Uint maxpat)
{

    inittree();
    initclock();

    if (maxpat > textlen) {
        ERROR("Max pattern length must be smaller than the text length");
    }

    iterate_search_patterns(trials, minpat, maxpat);

    FILE *fp = open_append(path);
    print_statistics(fp, trials);
    printtime();

    fclose(fp);
}
