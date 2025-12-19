#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cek_stunting.h"
#include "utils.h"

#define FILE_STUNTING "db/data_stunting.txt"
#define FILE_TEMP     "db/tmp_stunting.txt"
#define MAX_DATA 100

/* =======================
   STRUCT DATA
   ======================= */
typedef struct {
    int id;
    char nama[50];
    int umur;
    float tinggi;
    char status[20];
} Stunting;

/* =======================
   HITUNG STATUS
   ======================= */
void tentukanStatus(Stunting *s) {
    if (s->tinggi < 90)
        strcpy(s->status, "STUNTING");
    else
        strcpy(s->status, "NORMAL");
}

/* =======================
   TAMBAH DATA
   ======================= */
void tambahStunting() {
    clearScreen();
    printf("=== TAMBAH DATA STUNTING ===\n");

    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_STUNTING, "r");
    Stunting s;

    /* Cegah ID duplikat */
    if (f) {
        while (fscanf(f, "%d|%49[^|]|%d|%f|%19[^\n]\n",
                      &s.id, s.nama, &s.umur,
                      &s.tinggi, s.status) != EOF) {
            if (s.id == id) {
                printf("ID sudah ada!\n");
                fclose(f);
                pauseScreen();
                return;
            }
        }
        fclose(f);
    }

    f = fopen(FILE_STUNTING, "a");
    if (!f) return;

    s.id = id;
    inputString("Nama Anak   : ", s.nama, sizeof(s.nama));
    s.umur = inputInt("Umur (bulan): ");
    s.tinggi = inputFloat("Tinggi (cm): ");

    tentukanStatus(&s);

    fprintf(f, "%d|%s|%d|%.2f|%s\n",
            s.id, s.nama, s.umur,
            s.tinggi, s.status);

    fclose(f);
    printf("Data stunting berhasil ditambahkan!\n");
    pauseScreen();
}

/* =======================
   TAMPILKAN SEMUA
   ======================= */
void tampilkanSemuaStunting() {
    clearScreen();
    FILE *f = fopen(FILE_STUNTING, "r");
    Stunting data[MAX_DATA];
    int n = 0;

    if (!f) {
        printf("Belum ada data stunting.\n");
        pauseScreen();
        return;
    }

    while (fscanf(f, "%d|%49[^|]|%d|%f|%19[^\n]\n",
                  &data[n].id,
                  data[n].nama,
                  &data[n].umur,
                  &data[n].tinggi,
                  data[n].status) != EOF) {
        n++;
    }
    fclose(f);

    /* Sorting by nama (Bubble Sort) */
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(data[j].nama, data[j + 1].nama) > 0) {
                Stunting tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
        }
    }

    printf("=== DATA CEK STUNTING ===\n");
    for (int i = 0; i < n; i++) {
        printf("ID:%d | %s | Umur:%d | %.2f cm | %s\n",
               data[i].id,
               data[i].nama,
               data[i].umur,
               data[i].tinggi,
               data[i].status);
    }

    pauseScreen();
}

/* =======================
   CARI BY ID
   ======================= */
void cariStuntingById() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = kembali): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_STUNTING, "r");
    Stunting s;
    int found = 0;

    if (!f) {
        printf("Data kosong.\n");
        pauseScreen();
        return;
    }

    while (fscanf(f, "%d|%49[^|]|%d|%f|%19[^\n]\n",
                  &s.id, s.nama,
                  &s.umur, &s.tinggi,
                  s.status) != EOF) {
        if (s.id == id) {
            printf("\nDATA DITEMUKAN\n");
            printf("Nama   : %s\n", s.nama);
            printf("Umur   : %d bulan\n", s.umur);
            printf("Tinggi : %.2f cm\n", s.tinggi);
            printf("Status : %s\n", s.status);
            found = 1;
            break;
        }
    }

    fclose(f);
    if (!found) printf("Data tidak ditemukan.\n");
    pauseScreen();
}

/* =======================
   CARI BY NAMA
   ======================= */
void cariStuntingByNama() {
    clearScreen();
    char key[50];
    inputString("Masukkan nama: ", key, sizeof(key));

    FILE *f = fopen(FILE_STUNTING, "r");
    Stunting s;
    int found = 0;

    if (!f) {
        printf("Data kosong.\n");
        pauseScreen();
        return;
    }

    printf("\nHASIL PENCARIAN\n");
    while (fscanf(f, "%d|%49[^|]|%d|%f|%19[^\n]\n",
                  &s.id, s.nama,
                  &s.umur, &s.tinggi,
                  s.status) != EOF) {
        if (strstr(s.nama, key)) {
            printf("ID:%d | %s | %s\n",
                   s.id, s.nama, s.status);
            found = 1;
        }
    }

    fclose(f);
    if (!found) printf("Data tidak ditemukan.\n");
    pauseScreen();
}

/* =======================
   SUBMENU LIHAT
   ======================= */
void lihatStunting() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|       Data Stunting     |\n");
printf("|=========================|\n");
printf("|1.Lihat Semua            |\n");
printf("|-------------------------|\n");
printf("|2.Cari Stunting By Id    |\n");
printf("|-------------------------|\n");
printf("|3.Cari Stunting By Nama  |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tampilkanSemuaStunting();
        else if (p == 2) cariStuntingById();
        else if (p == 3) cariStuntingByNama();

    } while (p != 0);
}

/* =======================
   EDIT DATA
   ======================= */
void editStunting() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_STUNTING, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Stunting s;
    int found = 0;

    if (!f || !tmp) return;

    while (fscanf(f, "%d|%49[^|]|%d|%f|%19[^\n]\n",
                  &s.id, s.nama,
                  &s.umur, &s.tinggi,
                  s.status) != EOF) {
        if (s.id == id) {
            found = 1;
            inputString("Nama baru   : ", s.nama, sizeof(s.nama));
            s.umur = inputInt("Umur baru   : ");
            s.tinggi = inputFloat("Tinggi baru: ");
            tentukanStatus(&s);
        }
        fprintf(tmp, "%d|%s|%d|%.2f|%s\n",
                s.id, s.nama,
                s.umur, s.tinggi,
                s.status);
    }

    fclose(f);
    fclose(tmp);
    remove(FILE_STUNTING);
    rename(FILE_TEMP, FILE_STUNTING);

    printf(found ? "Data berhasil diubah!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* =======================
   HAPUS DATA
   ======================= */
void hapusStunting() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_STUNTING, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Stunting s;
    int found = 0;

    if (!f || !tmp) return;

    while (fscanf(f, "%d|%49[^|]|%d|%f|%19[^\n]\n",
                  &s.id, s.nama,
                  &s.umur, &s.tinggi,
                  s.status) != EOF) {
        if (s.id == id) {
            found = 1;
            continue;
        }
        fprintf(tmp, "%d|%s|%d|%.2f|%s\n",
                s.id, s.nama,
                s.umur, s.tinggi,
                s.status);
    }

    fclose(f);
    fclose(tmp);
    remove(FILE_STUNTING);
    rename(FILE_TEMP, FILE_STUNTING);

    printf(found ? "Data berhasil dihapus!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* =======================
   MENU UTAMA
   ======================= */
void menuStunting() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|       Stunting          |\n");
printf("|=========================|\n");
printf("|1.Tambah Data Stunting   |\n");
printf("|-------------------------|\n");
printf("|2.Lihat Data Stunting    |\n");
printf("|-------------------------|\n");
printf("|3.Edit Data Stunting     |\n");
printf("|-------------------------|\n");
printf("|4.Hapus Data Stunting    |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");;

        p = inputInt("Pilih: ");

        if (p == 1) tambahStunting();
        else if (p == 2) lihatStunting();
        else if (p == 3) editStunting();
        else if (p == 4) hapusStunting();

    } while (p != 0);
}
