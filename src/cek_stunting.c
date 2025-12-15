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
