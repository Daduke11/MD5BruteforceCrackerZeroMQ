#pragma once
#ifndef md5_h
#define md5_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

unsigned f0(unsigned abcd[]);
unsigned f1(unsigned abcd[]);
unsigned f2(unsigned abcd[]);
unsigned f3(unsigned abcd[]);
unsigned *calcKs(unsigned *k);
unsigned rol(unsigned v, short amt);
unsigned *md5(const char *msg, int mlen);

typedef union uwb {
	unsigned w;
	unsigned char b[4];
} WBunion;


#endif