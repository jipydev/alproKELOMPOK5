#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cek_stunting.h"
#include "utils.h"

#define FILE_STUNTING "db/stunting.txt"
#define FILE_TMP      "db/tmp_stunting.txt"

typedef struct {
    int id;
    char nama[50];
    int umur_bulan;
    float tinggi;
    char tanggal[20];
    char status[20];
} Stunting;


   //LOGIKA STUNTING//
 
void tentukanStatusStunting(Stunting *s) {
    float standar = (s->umur_bulan * 0.5f) + 50;

    if (s->tinggi < standar)
        strcpy(s->status, "STUNTING");
    else
        strcpy(s->status, "NORMAL");
}

