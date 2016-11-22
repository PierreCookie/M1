   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.24  07/01/05          */
   /*                                                     */
   /*                     MAIN MODULE                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Moved UserFunctions and EnvUserFunctions to    */
/*            the new userfunctions.c file.                  */
/*                                                           */
/*************************************************************/

/***************************************************************************/
/*                                                                         */
/* Permission is hereby granted, free of charge, to any person obtaining   */
/* a copy of this software and associated documentation files (the         */
/* "Software"), to deal in the Software without restriction, including     */
/* without limitation the rights to use, copy, modify, merge, publish,     */
/* distribute, and/or sell copies of the Software, and to permit persons   */
/* to whom the Software is furnished to do so.                             */
/*                                                                         */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS */
/* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF              */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT   */
/* OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY  */
/* CLAIM, OR ANY SPECIAL INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES */
/* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN   */
/* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF */
/* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.          */
/*                                                                         */
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clips.h"
#include "setup.h"
#include "sysdep.h"
#include "envrnmnt.h"
#include "extnfunc.h"
#include "commline.h"

int main(int,char *[]);
void UserFunctions(void);
void EnvUserFunctions(void *);

/****************************************/
/* main: Starts execution of the expert */
/*   system development environment.    */
/****************************************/

int main(int argc, char *argv[])
{
	void *theEnv;
	theEnv = CreateEnvironment();
	RerouteStdin(theEnv,argc,argv);
	CommandLoop(theEnv);

	/*==================================================================*/
	/* Control does not normally return from the CommandLoop function.  */
	/* However if you are embedding CLIPS, have replaced CommandLoop    */
	/* with your own embedded calls that will return to this point, and */
	/* are running software that helps detect memory leaks, you need to */
	/* add function calls here to deallocate memory still being used by */
	/* CLIPS. If you have a multi-threaded application, no environments */
	/* can be currently executing. If the ALLOW_ENVIRONMENT_GLOBALS     */
	/* flag in setup.h has been set to TRUE (the default value), you    */
	/* call the DeallocateEnvironmentData function which will call      */
	/* DestroyEnvironment for each existing environment and then        */
	/* deallocate the remaining data used to keep track of allocated    */
	/* environments. Otherwise, you must explicitly call                */
	/* DestroyEnvironment for each environment you create.              */
	/*==================================================================*/
	
	/* DeallocateEnvironmentData(); */
	/* DestroyEnvironment(theEnv); */
	
	return(-1);
}

/************************************/
/******* CLIPS User Functions *******/
/************************************/

 void *getVarEnv()
 {
	char *name;
	void *returnValue;
	 
	name = RtnLexeme(1);
	if (getenv(name) == NULL) return(AddSymbol("undefined"));
	returnValue = AddSymbol(getenv(name));
	return(returnValue);
 }

// http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int levenshtein()
{
	char *s1;
	char *s2;
	
	s1 = RtnLexeme(1);
	s2 = RtnLexeme(2);
	
    unsigned int s1len, s2len, x, y, lastdiag, olddiag;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int column[s1len+1];
    for (y = 1; y <= s1len; y++) column[y] = y;
    for (x = 1; x <= s2len; x++)
	{
        column[0] = x;
        for (y = 1, lastdiag = x-1; y <= s1len; y++)
		{
            olddiag = column[y];
            column[y] = MIN3(column[y]+1, column[y-1]+1, lastdiag+(s1[y-1] == s2[x-1] ? 0 : 1));
            lastdiag = olddiag;
        }
    }
    return(column[s1len]);
}

void UserFunctions()
{
	DefineFunction2("getVarEnv", 's', PTIF getVarEnv, "getVarEnv", "11s");
	DefineFunction2("levenshtein", 'i', PTIF levenshtein, "levenshtein", "22s");
}
 
void EnvUserFunctions(void *theEnv) {}

