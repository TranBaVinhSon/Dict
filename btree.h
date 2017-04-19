/*
 * $Id: btree.h,v 1.11 2011-06-13 19:34:46 mark Exp $
 * 
 * Standard btree header
 * Defines all API functions
 *
 * Copyright (C) 2003, 2004 Mark Willson.
 *
 * This file is part of the B Tree library.
 *
 * The B Tree library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * The B Tree library  is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the B Tree library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "bt.h"

extern int btcls(BTA *); //dong mot btree
extern BTA *btcrt(char *,int, int); //tao 1 b-tree voi btcrt(ten file , 0, 0) 2 so 0, 0 la mac dinh cho UNIX
extern int btchgr(BTA *,char *); 
extern int btcrtr(BTA *,char *);
extern int btdups(BTA *,BTint);
extern int btinit(void);
extern BTA *btopn(char *,int,int); //mo 1 btree da co san voi btopn(ten file, 0, 0) 2 so 0, 0 la mac dinh UNIX
//===================================================================================================
//lam viec voi kieu du khoa la char va value laf BTint (day la mot dang cua kieu long long)
extern int bdbug(BTA *,char *,BTint);
extern int bfndky(BTA *,char *,BTint *);//tim key co hay k
extern int binsky(BTA *,char *,BTint);
extern int bnxtky(BTA *,char *,BTint *);
extern int bprvky(BTA *,char *,BTint *);
extern int bdelky(BTA *,char *);
extern int btdelr(BTA*,char *);
extern void btcerr(int *,int *,char *,char *);
extern int bupdky(BTA *,char *,BTint);
//======================================================================================================
//lam viec voi kieu du lieu co key la char va value la char
extern int btins(BTA *,char *,char *,int); 
/* insert key va value vao btree btins(BTA, key, value, so ki tu max cua value)*/
extern int btsel(BTA *,char *,char *,int,int *); //search
/* in ra value khi biet key btsel(BTA, key, value (bien luu du lieu), do dai toi da cho value, do dai ly thuyet value nhan dc)*/
extern int btupd(BTA *,char *,char *,int); //edit lai value
/* cap nhat du lieu cho khoa key giong nhu btupd(BTA, key(co san), value moi cua key, do dai max cua value)*/
extern int btdel(BTA *,char *);//xoa key
extern int btselp(BTA *,char *,char *,int,int *);
/* xac dinh du lieu cho khoa truoc key cu phap giong btsel */
extern int btrecs(BTA *,char *,int *);

extern int btlock(BTA *);
extern int btunlock(BTA *);

extern int btpos(BTA *,int);//duyet cay
extern int btseln(BTA *,char *,char *,int,int *);
/* xac dinh du lieu cho khoa tiep theo cau key cu phap giong btsel */
extern int btthresh(BTA*, int);












