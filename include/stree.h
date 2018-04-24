/*
 * Copyright by Stefan Kurtz (C) 1999-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 *
 * ======================================
 *
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#ifndef STREE_H
#define STREE_H

#include "types.h"
#include "basedef.h"
#include "externs.h"


// Determines the index of a vertex. V is the adress pointing to the vertex's
// position in the text. By subtracting the adress of the tree, the index is
// obtained.
#define INDEX(V)            ((Uint) ((V) - stree))

// Instead of storing seperately if a vertex is a leaf, a rightmostchild or
// unevaluated, this information is encoded in the MSB and the second MSB of
// the number.

// Bit determining if the vertex is a leaf. This is storde in the first
// integer, ie. its left pointer.
#define LEAFBIT                 FIRSTBIT

// Bit determining if the node is the rightmost child of its parent. This is
// stored in the first integer, ie its left pointer.
#define RIGHTMOSTCHILDBIT       SECONDBIT

// Bit determining if the inner node is unevaluated. This is stored in the
// second integer of the vertex, ie. its first child.
#define UNEVALUATEDBIT          FIRSTBIT

///////////////////////////////////////////////////////////////////////////////
// Getters

// This is the left pointer, defined as the minimum leaf under P, plus the
// length of the path to its parent.  To retrieve the edge labels in constant
// time, it suffices to store the left pointer for all nodes.
//
// A node is referenced by the index(lp(u)), disregarding the first and second
// bit.
#define GET_LP(V)               ((*(V)) & ~(LEAFBIT | RIGHTMOSTCHILDBIT))

// For each branching vertex, we additionally need constant time access to the
// child of a vertex with the smallest left pointer. This accesss is provided
// by the following reference.
//
// The algorithm only needs access to this child in order to evaluate all
// children of V. The children are sorted according to first character, and
// laid out in `stree` sequentially, so the algorithm can iterate over these,
// setting RIGHTMOSTBIT to the last vertex.
#define GET_FIRSTCHILD(V)       (*((V) + 1))

// The left boundry of the remaining suffixes
#define GET_LEFTB(V)            (suffixes + *(V))

// The right boundry of the remaining suffixes, stripping of the first bit.
//
// Note that the right boundry is defined as the adrees starting at suffixes,
// with the offset of the first child of V.
#define GET_RIGHTB(V)           (suffixes + (GET_FIRSTCHILD(V) & ~UNEVALUATEDBIT))

// startposition of suffix
#define SUFFIX_INDEX(L)         ((Uint) (*(L) - wtext))

// The lp number of an unevaluated vertex V. This is just the index for the
// left-bound of V.
#define GET_LP_UNEVAL(V)        SUFFIX_INDEX(GET_LEFTB(V))

// Given a LP value and a vertex V, set the lp-value for V, by ORing the two
// integers. V is first ANDed with its second bit, to reset all bits except
// the rightmost one.
#define SET_LP(V, LP)           *(V) = (*(V) & RIGHTMOSTCHILDBIT) | (LP)

// Set the first child of V. C corresponds to an index in the stree table.
#define SET_FIRSTCHILD(V, C)    GET_FIRSTCHILD(V) = C

// Set the leaf number of V to N. The first bit is also set to 1, since this is
// a leaf.
#define SET_LEAF(V, N)          *(V) = (N) | LEAFBIT

// Each branching vertex requires two integers, one for the left boundary and
// one for the right boundary
#define BRANCHWIDTH             UINT(2)

// Each vertex needs two integers allocated for it, one for its left suffix
// boundary and one for its left suffix boundary. This macro sets the left and
// the right boundary for the two following positions in P.
#define SET_BOUNDARIES(P, L, R)\
    *(P) = (Uint) ((L) - suffixes);\
    *((P) + 1) = ((R) - suffixes) | UNEVALUATEDBIT


// Retrieve the MSB of LP(V)
#define IS_LEAF(V)              ((*(V)) & LEAFBIT)

// Retrieve the second MSB of LP(V)
#define IS_RIGHTMOST(V)         ((*(V)) & RIGHTMOSTCHILDBIT)

// Retrieve the MSB of FIRSTCHILD(V)
#define IS_UNEVALUATED(V)       ((*((V) + 1)) & UNEVALUATEDBIT)

void create_root_leaf(Wchar firstchar, Wchar **left);

void create_inner_vertex(
        Wchar firstchar,
        Wchar **left,
        Wchar **right,
        bool root
    );

Uint create_leaf_vertex(Wchar firstchar, Wchar **left, bool root);

Uint create_sentinel_vertex(Wchar **right, Uint **previousnode);

void init_root_children();

#endif
