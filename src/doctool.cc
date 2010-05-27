/*

   Copyright (c) 2004-2005 PathScale, Inc.  All rights reserved.
   PathDB is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation version 3

   PathDB is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with PathDB; see the file COPYING.  If not, write to the Free
   Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

file: doctool.cc
created on: Fri Dec 17 11:03:24 PDT 2004
author: James Strother <jims@pathscale.com>

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "doctext.h"

#define MAX_LEN 72
#define NAME_LEN 20
#define MAX_WARN 5

#define SEPR "----------------------------------------------------"

void error(const char* s)
{
   fprintf(stderr, "doctool error: %s\n", s);
   exit(1);
}

void validate (const char** entries)
{
   const char* s = *entries;
   int num_warnings = 0;

   /* traverse all entries */
   while (s != NULL)
   {
      int lineno = 1;
      char name[NAME_LEN+1];
      const char* n;

      /* grab a name */
      strncpy(name, s, NAME_LEN); 
      name[NAME_LEN] = '\0';

      /* check each line */
      for (;;)
      {
         /* grab next */
         n = strchr(s, '\n');
         if (n == NULL) break; 

         /* check doubled */
         if (*(n+1) == '\n')
         {
            if (++num_warnings > MAX_WARN)
            {
               printf("doctool error: suppressing further warnings\n");
               goto validate_exit;
            }
            printf("doctool warning: doubled newlines: ");
            printf("line %d of %s\n", lineno+1, name);
            break;
         }

         /* check columns */
         if (n - s > MAX_LEN)
         {
            if (++num_warnings > MAX_WARN)
            {
               printf("doctool error: suppressing further warnings\n");
               goto validate_exit;
            }
            printf("doctool warning: line too long: ");
            printf("line %d of %s\n", lineno, name);
            break;
         }

         /* next line */
         s = n+1;
         lineno++;
      }
     
      /* iterate */ 
      entries++;
      s=*entries;
   }

validate_exit:
   /* separated warnings from text */
   if (num_warnings > 0)
      printf("%s\n",SEPR);
}

int main(int argc, char* argv[])
{
   pathdb_help_entry_t* h;
   unsigned found_entry=0;

   /* check arguments */
   if (argc != 2)
      error("expects one argument");

   /* validate all entries */
   validate(PATHDB_HELP_ENTRIES);

   /* search for particular key */
   h = PATHDB_HELP_TABLE;
   while (h->name != NULL)
   {
      if (!strcmp(h->name,argv[1]))
      {
        found_entry = 1;
        break;
      }
      h++;
   }

   /* check for lost entry */
   if (!found_entry)
      error("key not found");
  
   /* print out contents */
   printf(PATHDB_HELP_ENTRIES[h->id]);

   /* success */
   return 0;
}

