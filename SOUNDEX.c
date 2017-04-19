#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "btree.h"
void soundex(char *chAlphaName, char *strResult)
{
     int i;
     int j = 0;
     char SCode = '0';
     char PrevCode = '0';
     char CharTemp = '0';

     for (i = 0; i < strlen(chAlphaName); i++)
     {
          chAlphaName[i]=tolower(chAlphaName[i]);
     }

     for (i = 0; (i < strlen(chAlphaName) && j < 4); i++)
     {
          CharTemp = chAlphaName[i];

          switch(CharTemp)
          {
               case 'r':
                    SCode = '6';
                    break;
               case 'm':
               case 'n':
                    SCode='5';
                    break;
               case 'l':
                    SCode='4';
                    break;
               case 'd':
               case 't':
                    SCode='3';
                    break;
               case 'c':
               case 'g':
               case 'j':
               case 'k':
               case 'q':
               case 's':
               case 'x':
               case 'z':
                    SCode = '2';
                    break;
               case 'b':
               case 'f':
               case 'p':
               case 'v':
                    SCode = '1';
                    break;
               default:
                    SCode = '0';
                    break;
          }

          if (SCode > '0' || j==0)
          {
               //SCode la chu cai dau tien
               if (j == 0)
               {
                    strResult[j] = chAlphaName[j];
                   // strResult[j+1] = '\0';

                    j++;
               }
               else if (SCode != PrevCode)
                    {
                         strResult[j] = SCode;
                    //     strResult[j+1] = '\0';

                         j++;
                    }
          }


          if (CharTemp == 'h' || CharTemp == 'w')
          {
               SCode = PrevCode;
          }

          PrevCode = SCode;
          SCode = '0';

     }

     for (i = j; i < 4; i++)
     {
          strResult[i] = '0' ;
     }

     strResult[i]='\0';
}


BTA *xulysoundex(BTA *p, char *word)
{
	int i = strlen(word), k;
	char soundexCode[100];

     	soundex(word, soundexCode);


	char value[100000];
	int size, j;
	k = btsel(p, soundexCode, value, 100000, &size);
	if (k == 0)
		{
			strcat(value, "\n");
			strcat(value, word);

			btupd(p, soundexCode, value, strlen(value)+1);
		}
	else
		{
			strcpy(value, "Cac tu gan dung la:  \n");
			strcat(value, word);
			btins(p, soundexCode, value, strlen(value)+1);
		}
   	return p;
}


BTA *taotudien()
{
  BTA *p;
  p = btcrt("tdanhviet", 0, 0);
  return p;
}

BTA *taosoundex()
{
    BTA *p;
  p = btcrt("tdanhvietsoundex", 0,0);
  return p;
}

char *tachXau(char *s,char **val){
  int i=0,j=0;
  char *key,*value;
  key = (char*)malloc(30*sizeof(char));
  value = (char*)malloc(1000*sizeof(char));
  while(s[i++]!='{');
  while(s[i]!='}')key[j++] = s[i++];
  j = 0;
  while(s[i++]!='{');
  while(s[i]!='}')value[j++] = s[i++];
  *val = value ;
  return key;
}

void *nhandulieu(FILE *f,BTA *p,BTA *q)
{
  char *s,*key,*val;
  s=(char *)malloc(1000);
  int i=0;
  while(fgets(s, 1000, f)!=NULL){
    if ((s[0]=='/'&&s[1]  =='/')||s[0]=='['||s[0]==']'||strlen(s)<6) continue;
    s[strlen(s)-1] = '\0';
    key = tachXau(s,&val);
    puts(key);
    puts(val);
    btins(p, key, val,strlen(val)+1);
    q = xulysoundex(q,key);
  }
  return f;
}
void make_file(char filename[]){
  FILE *f = fopen(filename,"r");
  BTA *p = taotudien();
  BTA *q = taosoundex();
  f = nhandulieu(f,p,q);
  btcls(p);
  btcls(q);
  fclose(f);
}





/*

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
*/