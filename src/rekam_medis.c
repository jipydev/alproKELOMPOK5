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

/* =======================
   LOAD & SAVE (PERSISTENSI)
   ======================= */
void loadRekam() {
    FILE *f = fopen(FILE_REKAM, "r");
    jumlahRekam = 0;

    if (!f) return;

    while (fscanf(f, "%d|%49[^|]|%99[^\n]\n",
           &dataRekam[jumlahRekam].id,
           dataRekam[jumlahRekam].nama,
           dataRekam[jumlahRekam].keluhan) != EOF) {

        jumlahRekam++;
        if (jumlahRekam >= MAX_REKAM) break;
    }
    fclose(f);
}

/* =======================
   SEARCHING (SEQUENTIAL)
   ======================= */
int cariIndexById(int id) {
    for (int i = 0; i < jumlahRekam; i++) {
        if (dataRekam[i].id == id)
            return i;
    }
    return -1;
}

/* =======================
   SORTING (BUBBLE SORT)
   ======================= */
void sortRekamByNama() {
    for (int i = 0; i < jumlahRekam - 1; i++) {
        for (int j = 0; j < jumlahRekam - i - 1; j++) {
            if (strcmp(dataRekam[j].nama, dataRekam[j + 1].nama) > 0) {
                Rekam temp = dataRekam[j];
                dataRekam[j] = dataRekam[j + 1];
                dataRekam[j + 1] = temp;
            }
        }
    }
}