
#include "stree.h"

Uint    *next_element, root_children[MAX_CHARS + 1];

void init_root_children()
{
    Uint *cursor;
    for(cursor = root_children; cursor <= root_children + MAX_CHARS; cursor++) {
        *cursor = UNDEFINEDSUCC;
    }
}


Uint create_leaf_vertex(Wchar first, Wchar **left, bool root)
{
    Uint leafnum = SUFFIX_INDEX(left);
    *next_element = WITH_LEAFBIT(leafnum);
    if (root) {
        root_children[first] = leafnum | LEAFBIT;
    }
    next_element += LEAF_VERTEXSIZE;
    return leafnum;
}



void create_inner_vertex(Wchar first, Wchar **leftb, Wchar **rightb, bool root)
{
    *next_element = leftb - suffixes;
    CHILD(next_element) = WITH_UNEVALBIT(rightb - suffixes);

    if (root) {
        root_children[first] = INDEX(next_element);
    }
    next_element += INNER_VERTEXSIZE;
}


Uint create_sentinel_vertex(Wchar **right, Uint **previousnode)
{
    Uint leafnum = create_leaf_vertex('\0', right + 1, false);
    *previousnode = next_element;
    return leafnum;
}


void stree_destroy()
{
    free(wtext);
    free(sortbuffer);
    free(vertices);
    free(suffixes);
    free(leaf_nums);
    free(recurse_suffixes);
}
