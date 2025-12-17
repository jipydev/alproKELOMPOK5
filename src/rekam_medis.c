#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rekam_medis.h"
#include "utils.h"

#define FILE_REKAM "db/rekam_medis.txt"
#define FILE_TEMP  "db/temp.txt"

/* ================= TAMBAH ================= */
void tambahRekam() {
    clearScreen();
    printf("=== TAMBAH REKAM MEDIS ===\n");

    int id = inputInt("Masukkan ID Rekam (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_REKAM, "r");
    Rekam r;

    /* Cek ID duplikat */
    if (f) {
        while (fscanf(f, "%d|%49[^|]|%99[^\n]\n",
                       &r.id, r.nama, r.keluhan) != EOF) {
            if (r.id == id) {
                printf("ID sudah ada!\n");
                fclose(f);
                pauseScreen();
                return;
            }
        }
        fclose(f);
    }

    f = fopen(FILE_REKAM, "a");
    if (!f) return;

    r.id = id;
    inputString("Nama Pasien: ", r.nama, sizeof(r.nama));
    inputString("Keluhan: ", r.keluhan, sizeof(r.keluhan));

    fprintf(f, "%d|%s|%s\n", r.id, r.nama, r.keluhan);
    fclose(f);

    printf("Rekam medis berhasil ditambahkan!\n");
    pauseScreen();
}

/* ================= LIHAT ================= */
void tampilkanSemuaRekam() {
    clearScreen();
    FILE *f = fopen(FILE_REKAM, "r");
    Rekam r;

    if (!f) {
        printf("Belum ada data.\n");
        pauseScreen();
        return;
    }

    printf("=== DATA REKAM MEDIS ===\n");
    while (fscanf(f, "%d|%49[^|]|%99[^\n]\n",
                  &r.id, r.nama, r.keluhan) != EOF) {
        printf("ID:%d | %s | Keluhan: %s\n", r.id, r.nama, r.keluhan);
    }

    fclose(f);
    pauseScreen();
}

void cariRekamById() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = kembali): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_REKAM, "r");
    Rekam r;
    int found = 0;

    while (f && fscanf(f, "%d|%49[^|]|%99[^\n]\n",
                       &r.id, r.nama, r.keluhan) != EOF) {
        if (r.id == id) {
            printf("Ditemukan:\n");
            printf("ID:%d\nNama:%s\nKeluhan:%s\n",
                   r.id, r.nama, r.keluhan);
            found = 1;
            break;
        }
    }

    if (!found) printf("Data tidak ditemukan.\n");
    if (f) fclose(f);
    pauseScreen();
}

void cariRekamByNama() {
    clearScreen();
    char key[50];
    inputString("Masukkan nama pasien: ", key, sizeof(key));

    FILE *f = fopen(FILE_REKAM, "r");
    Rekam r;
    int found = 0;

    while (f && fscanf(f, "%d|%49[^|]|%99[^\n]\n",
                       &r.id, r.nama, r.keluhan) != EOF) {
        if (strstr(r.nama, key)) {
            printf("ID:%d | %s | Keluhan:%s\n",
                   r.id, r.nama, r.keluhan);
            found = 1;
        }
    }

    if (!found) printf("Tidak ditemukan.\n");
    if (f) fclose(f);
    pauseScreen();
}

void lihatRekam() {
    int p;
    do {
        clearScreen();
        printf("=== LIHAT REKAM MEDIS ===\n");
        printf("1. Tampilkan Semua\n");
        printf("2. Cari berdasarkan ID\n");
        printf("3. Cari berdasarkan Nama\n");
        printf("0. Kembali\n");

        p = inputInt("Pilih: ");

        if (p == 1) tampilkanSemuaRekam();
        else if (p == 2) cariRekamById();
        else if (p == 3) cariRekamByNama();

    } while (p != 0);
}

/* ================= EDIT ================= */
void editRekam() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_REKAM, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Rekam r;
    int found = 0;

    while (f && fscanf(f, "%d|%49[^|]|%99[^\n]\n",
                       &r.id, r.nama, r.keluhan) != EOF) {
        if (r.id == id) {
            found = 1;
            inputString("Nama baru: ", r.nama, sizeof(r.nama));
            inputString("Keluhan baru: ", r.keluhan, sizeof(r.keluhan));
        }
        fprintf(tmp, "%d|%s|%s\n", r.id, r.nama, r.keluhan);
    }

    if (f) fclose(f);
    fclose(tmp);

    remove(FILE_REKAM);
    rename(FILE_TEMP, FILE_REKAM);

    printf(found ? "Data diupdate!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* ================= HAPUS ================= */
void hapusRekam() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    FILE *f = fopen(FILE_REKAM, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Rekam r;
    int found = 0;

    while (f && fscanf(f, "%d|%49[^|]|%99[^\n]\n",
                       &r.id, r.nama, r.keluhan) != EOF) {
        if (r.id == id) {
            found = 1;
            continue;
        }
        fprintf(tmp, "%d|%s|%s\n", r.id, r.nama, r.keluhan);
    }

    if (f) fclose(f);
    fclose(tmp);

    remove(FILE_REKAM);
    rename(FILE_TEMP, FILE_REKAM);

    printf(found ? "Data dihapus!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* ================= MENU ================= */
void menuRekam() {
    int p;
    do {
        clearScreen();
        printf("=== CRUD REKAM MEDIS ===\n");
        printf("1. Tambah\n");
        printf("2. Lihat\n");
        printf("3. Edit\n");
        printf("4. Hapus\n");
        printf("0. Kembali\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahRekam();
        else if (p == 2) lihatRekam();
        else if (p == 3) editRekam();
        else if (p == 4) hapusRekam();

    } while (p != 0);
}