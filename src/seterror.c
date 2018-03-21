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

//\Ignore{

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include "types.h"

//}

/*EE
  This module implements a simple mechanism to write error messages
  into a global buffer, and to output this buffer when required.
  We also maintain a global error code.
*/

/*
  The buffer to write the error message is of the following size.
*/

#define MAXERRORMSG 1024

static Sint errorcode = 0;
static char errormessage[MAXERRORMSG+1];

/*EE
  The following function returns the size of the buffer, and thus
  the maximal length of an error message.
*/

Sint maxerrormsg(void)
{
  return (Sint) MAXERRORMSG;
}

/*EE
  The following function returns a reference to the buffer for storing
  or retrieving the error message.
*/

char *messagespace(void)
{
  return errormessage; // write message with sprintf into this array
}

/*EE
  The following function sets the error code. It should be
  called in the innermost function in which the error occurs.
*/

void seterror(Sint code)
{
  errorcode = code;
}

/*EE
  The following function returns the error code.
*/

Sint geterror(void)
{
  return errorcode;
}

/*EE
  The following function resets the error code to 0.
*/

void reseterror(void)
{
  errorcode = 0;
}
