#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rekam_medis.h"
#include "utils.h"

/* =======================
   KONFIGURASI
   ======================= */
#define MAX_REKAM 100
#define FILE_REKAM "db/rekam_medis.txt"

/* =======================
   ARRAY OF STRUCT (DATA TEMPORER)
   ======================= */
Rekam dataRekam[MAX_REKAM];
int jumlahRekam = 0;
