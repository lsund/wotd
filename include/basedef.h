#ifndef PREPROCDEF_H
#define PREPROCDEF_H


///////////////////////////////////////////////////////////////////////////////
// Fields

#define ITER                    1

// Each branching vertex requires two integers
#define BRANCHWIDTH             UintConst(2)

#define MAXTEXTLEN              UintConst(715827882)

#define MAXPATTERNLEN           1024

#define MAX_PATTERNS            15000

// undefined reference
#define UNDEFREFERENCE          UINT_MAX

// undefined successor
#define UNDEFINEDSUCC           UINT_MAX

// Transform bytes into megabytes
#define MEGABYTES(V)            ((double) (V)/((UintConst(1) << 20) - 1))

#define MAX_CHARS               10000

// Is a character larger than 255
#define ISSPECIAL(C)    ((C) >= (Uchar) UCHAR_MAX)

#endif
