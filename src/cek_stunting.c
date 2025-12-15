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
//TAMBAH DATA//
void tambahStunting() {
    clearScreen();
    printf("=== TAMBAH DATA STUNTING BALITA ===\n");

    Stunting s;
    int id = inputInt("ID: ");

    FILE *f = fopen(FILE_STUNTING, "r");

    /* CEK ID DUPLIKAT */
    if (f) {
        while (fscanf(f,
            "%d|%49[^|]|%d|%f|%19[^|]|%19[^\n]\n",
            &s.id, s.nama, &s.umur_bulan,
            &s.tinggi, s.tanggal, s.status) != EOF) {

            if (s.id == id) {
                printf("ID sudah terdaftar!\n");
                fclose(f);
                pauseScreen();
                return;
            }
        }
        fclose(f);
    }
       s.id = id;
    inputString("Nama balita: ", s.nama, sizeof(s.nama));
    s.umur_bulan = inputInt("Umur (bulan): ");

    printf("Tinggi badan (cm): ");
    scanf("%f", &s.tinggi);
    while (getchar() != '\n');

    inputString("Tanggal pemeriksaan (contoh: 12-09-2025): ",
                s.tanggal, sizeof(s.tanggal));

    tentukanStatusStunting(&s);

    f = fopen(FILE_STUNTING, "a");
    if (!f) {
        printf("Gagal membuka file.\n");
        pauseScreen();
        return;
    }

    fprintf(f, "%d|%s|%d|%.1f|%s|%s\n",
            s.id, s.nama, s.umur_bulan,
            s.tinggi, s.tanggal, s.status);
  fclose(f);
    printf("Data berhasil disimpan!\n");
    pauseScreen();
}

   //TAMPILKAN SEMUA//
 
void tampilkanSemuaStunting() {
    clearScreen();
    FILE *f = fopen(FILE_STUNTING, "r");
    if (!f) {
        printf("Belum ada data.\n");
        pauseScreen();
        return;
    }

    Stunting s;
    printf("=== DATA STUNTING BALITA ===\n");

    while (fscanf(f,
        "%d|%49[^|]|%d|%f|%19[^|]|%19[^\n]\n",
        &s.id, s.nama, &s.umur_bulan,
        &s.tinggi, s.tanggal, s.status) != EOF) {

        printf("ID:%d | %s | %d bln | %.1f cm | %s | %s\n",
               s.id, s.nama, s.umur_bulan,
               s.tinggi, s.status, s.tanggal);
    }




