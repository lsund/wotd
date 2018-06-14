
#include "init.h"

Wchar       *wtext,
            *sentinel,
            characters[MAX_CHARS + 1],
            **suffixes,
            **recurse_suffixes;

Uint        textlen,
            alphasize,
            *vertices,
            streesize,
            *next_element,
            sortbufferwidth,
            max_sortbufferwidth,
            suffixhead_count[MAX_CHARS + 1],
            n_recursed;


bool    root_evaluated;


static void init_alphabet()
{
    Uint i;
    get_characters(characters, &alphasize);
    for (i = 0; i <= MAX_CHARS; i++) {
        suffixhead_count[i] = 0;
    }
}


static void init_stree()
{
    root_evaluated = false;
    streesize      = EXTENSION_SIZE;
    ALLOC(vertices, vertices, Uint, streesize);
    ALLOC(suffixes, NULL, Wchar *, textlen + 1);
    next_element   = vertices;

    recurse_suffixes = malloc(sizeof(Wchar *) * 5000);
    leaf_nums = malloc(sizeof(Uint) * 50000);

    n_recursed = 0;
}


static void init_sortbuffer()
{
    sortbufferwidth    = 0;
    max_sortbufferwidth = textlen >> 8;
}


void init()
{
    init_alphabet();
    init_sortbuffer();
    init_stree();
}

