#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jadwal_kegiatan.h"
#include "utils.h"

#define FILE_JADWAL "db/jadwal_kegiatan.txt"
#define FILE_TEMP  "db/temp.txt"

/* ================= TAMBAH ================= */

void tambahJadwal() {
    clearScreen();
    printf("=== TAMBAH JADWAL KEGIATAN ===\n");

    int id = inputInt("Masukkan ID Jadwal (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_JADWAL, "r");
    Jadwal j;

    /* Cek ID duplikat */

     if (f) {
        while (fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                       &j.id, j.kegiatan, j.tanggal) != EOF) {
            if (j.id == id) {
                printf("ID sudah ada!\n");
                fclose(f);
                pauseScreen();
                return;
            }
        }
        fclose(f);
    }

     f = fopen(FILE_JADWAL, "a");
    if (!f) return;

      j.id = id;
    inputString("Nama Kegiatan: ", j.kegiatan, sizeof(j.kegiatan));
    inputString("Tanggal (contoh: 12-09-2025): ", j.tanggal, sizeof(j.tanggal));

    fprintf(f, "%d|%s|%s\n", j.id, j.kegiatan, j.tanggal);
    fclose(f);

    printf("Jadwal berhasil ditambahkan!\n");
    pauseScreen();
}

/* ================= LIHAT ================= */

void tampilkanSemuaJadwal() {
    clearScreen();
    FILE *f = fopen(FILE_JADWAL, "r");
    Jadwal j;

     if (!f) {
        printf("Belum ada data jadwal.\n");
        pauseScreen();
        return;

     }

    printf("=== DAFTAR JADWAL KEGIATAN ===\n");
    while (fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                  &j.id, j.kegiatan, j.tanggal) != EOF) {
        printf("ID:%d | %s | %s\n", j.id, j.kegiatan, j.tanggal);
    }

    fclose(f);
    pauseScreen();
}


void cariJadwalById() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = kembali): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_JADWAL, "r");
    Jadwal j;
    int found = 0;

    while (f && fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                       &j.id, j.kegiatan, j.tanggal) != EOF) {
        if (j.id == id) {

             printf("Ditemukan:\n");
            printf("ID:%d\nKegiatan:%s\nTanggal:%s\n",
                   j.id, j.kegiatan, j.tanggal);
            found = 1;
            break;
        }

         }

    if (!found) printf("Jadwal tidak ditemukan.\n");
    if (f) fclose(f);
    pauseScreen();

    }

void cariJadwalByNama() {
    clearScreen();
    char key[50];
    inputString("Masukkan nama kegiatan: ", key, sizeof(key));

    FILE *f = fopen(FILE_JADWAL, "r");
    Jadwal j;
    int found = 0;

    while (f && fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                       &j.id, j.kegiatan, j.tanggal) != EOF) {

                        if (strstr(j.kegiatan, key)) {
            printf("ID:%d | %s | %s\n",
                   j.id, j.kegiatan, j.tanggal);
            found = 1;
        }

          }

    if (!found) printf("Tidak ada jadwal ditemukan.\n");
    if (f) fclose(f);
    pauseScreen();

    }

void cariJadwalByTanggal() {
    clearScreen();

    char tanggal[30];
    inputString("Masukkan tanggal (contoh: 12-09-2025): ", tanggal, sizeof(tanggal));

    FILE *f = fopen(FILE_JADWAL, "r");
    Jadwal j;
    int found = 0;

    if (!f) {
        printf("Belum ada data jadwal.\n");
        pauseScreen();
        return;

         }

    printf("\n=== JADWAL PADA TANGGAL %s ===\n", tanggal);


      while (fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                  &j.id, j.kegiatan, j.tanggal) != EOF) {

                     if (strcmp(j.tanggal, tanggal) == 0) {
            printf("ID:%d | %s\n", j.id, j.kegiatan);
            found = 1;
        }

         }

    if (!found)
        printf("Tidak ada jadwal di tanggal tersebut.\n");

        fclose(f);
    pauseScreen();
}

void lihatJadwal() {
    int p;
    do {
        clearScreen();
        printf("=== LIHAT JADWAL KEGIATAN ===\n");
        printf("1. Tampilkan Semua\n");
        printf("2. Lihat Berdasarkan Tanggal\n");

         printf("3. Cari berdasarkan ID\n");
        printf("4. Cari berdasarkan Nama\n");
        printf("0. Kembali\n");


         p = inputInt("Pilih: ");

         if (p == 1) tampilkanSemuaJadwal();
        else if (p == 2) cariJadwalByTanggal();
        else if (p == 3) cariJadwalById();
        else if (p == 4) cariJadwalByNama();

 } while (p != 0);
}

/* ================= EDIT ================= */

void editJadwal() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

     FILE *f = fopen(FILE_JADWAL, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Jadwal j;
    int found = 0;

    while (f && fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                       &j.id, j.kegiatan, j.tanggal) != EOF) {

                        if (j.id == id) {
            found = 1;
            inputString("Nama kegiatan baru: ", j.kegiatan, sizeof(j.kegiatan));
            inputString("Tanggal baru: ", j.tanggal, sizeof(j.tanggal));

             }
        fprintf(tmp, "%d|%s|%s\n", j.id, j.kegiatan, j.tanggal);

         }

    if (f) fclose(f);
    fclose(tmp);


     remove(FILE_JADWAL);
    rename(FILE_TEMP, FILE_JADWAL);

    printf(found ? "Jadwal diperbarui!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}


/* ================= HAPUS ================= */

void hapusJadwal() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_JADWAL, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Jadwal j;
    int found = 0;

    while (f && fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                       &j.id, j.kegiatan, j.tanggal) != EOF) {
        if (j.id == id) {
            found = 1;
            continue;

             }
        fprintf(tmp, "%d|%s|%s\n", j.id, j.kegiatan, j.tanggal);

          }

    if (f) fclose(f);
    fclose(tmp);

     remove(FILE_JADWAL);
    rename(FILE_TEMP, FILE_JADWAL);

      printf(found ? "Jadwal dihapus!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* ================= MENU ================= */

void menuJadwal() {
    int p;
    do {
        clearScreen();
        printf("=== CRUD JADWAL KEGIATAN ===\n");
        printf("1. Tambah\n");
        printf("2. Lihat\n");
        printf("3. Edit\n");

        printf("4. Hapus\n");
        printf("0. Kembali\n");

         p = inputInt("Pilih: ");

 if (p == 1) tambahJadwal();
        else if (p == 2) lihatJadwal();
        else if (p == 3) editJadwal();
        else if (p == 4) hapusJadwal();

         } while (p != 0);
}
