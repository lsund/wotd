
#include "naive_search.h"

Uint textlen;

Wchar *wtext;


// Naively search `wtext` for the pattern beginning at address `start`, ending
// at address `end`.
bool naive_search(Wchar *start, Wchar *end)
{
    Uint m = (Uint) (end - start);
    Wchar *pattern = start;
    Uint j = 0;
    Uint k;

    for (Uint i = 0; i < textlen; i++) {
        k = i;
        for (j = 0; j < m; j++) {
            if (pattern[j] == wtext[k]) {
                k++;
            } else {
                break;
            }
        }
        if (j == m) {
            return true;
        }
    }
    return false;
}


Uint naive_find_all(Wchar *start, Wchar *end, Uint *numbers)
{
    Uint m = (Uint) (end - start);
    Wchar *pattern = start;
    Uint j = 0;
    Uint k;
    Uint n_found = 0;

    for (Uint i = 0; i < textlen; i++) {
        k = i;
        for (j = 0; j < m; j++) {
            if (pattern[j] == wtext[k]) {
                k++;
            } else {
                break;
            }
        }
        if (j == m) {
            numbers[n_found] = i;
            n_found++;
            continue;
        }
    }
    return n_found;
}
