#include "btree.h"
#include "soundex.c"
#include "soundex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//=========================================================




FILE *read_special(FILE *f)
{
     char c;
     while ((c=fgetc(f))!='\n');
     return f;
}

FILE *read_string(FILE *f, char *s)
{
     char c;
     int i=0, kt=0;
     while ((c=fgetc(f))!='}'){

     if ((kt==1)&&(c=='/')) f=read_special(f);
     else if ((kt==0)&&(c=='/')) kt=1;
     else{
          s[i]=c;
          i++;
     }

  }
     s[i]='\0';
     return f;
}

int nhandulieu(FILE *f, BTA *p, BTA *q)
{
  char key[1000], val[1000];


  char c;
  while ((c=fgetc(f))!='[');
  while (1){
     while ((c=fgetc(f))!='{');
     f=read_string(f, key);
     puts(key);


     while ((c=fgetc(f))!='{');
     f=read_string(f, val);
     puts(val);
     }

   while ((c=fgetc(f))!=',')
     if (c==']') return 0;

btins(p, key, val, 10000);
    q = xulysoundex(q,key);


}

void make_file(char filename[]){
  FILE *f = fopen(filename, "r");
  BTA *p = btcrt("tdanhviet", 0, 0);
  BTA *q = btcrt("tdanhvietsoundex", 0,0);
  nhandulieu(f,p,q);
  btcls(p);
  btcls(q);
  fclose(f);
}

