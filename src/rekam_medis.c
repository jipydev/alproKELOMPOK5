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

/* =======================
   TAMBAH DATA
   ======================= */
void tambahRekam() {
    clearScreen();
    loadRekam();

    int id = inputInt("Masukkan ID Rekam (0 = batal): ");
    if (id == 0) return;

    if (cariIndexById(id) != -1) {
        printf("ID sudah digunakan!\n");
        pauseScreen();
        return;
    }

    Rekam r;
    r.id = id;
    inputString("Nama Pasien : ", r.nama, sizeof(r.nama));
    inputString("Keluhan     : ", r.keluhan, sizeof(r.keluhan));

    dataRekam[jumlahRekam++] = r;
    saveRekam();

    printf("Data rekam medis berhasil ditambahkan!\n");
    pauseScreen();
}

/* =======================
   LIHAT DATA
   ======================= */
void tampilkanSemuaRekam() {
    clearScreen();
    loadRekam();
    sortRekamByNama();

    printf("=== DATA REKAM MEDIS ===\n");
    for (int i = 0; i < jumlahRekam; i++) {
        printf("ID:%d | %s | Keluhan:%s\n",
            dataRekam[i].id,
            dataRekam[i].nama,
            dataRekam[i].keluhan);
    }
    pauseScreen();
}

void cariRekamById() {
    clearScreen();
    loadRekam();

    int id = inputInt("Masukkan ID: ");
    int idx = cariIndexById(id);

    if (idx == -1) {
        printf("Data tidak ditemukan.\n");
    } else {
        printf("ID:%d\nNama:%s\nKeluhan:%s\n",
            dataRekam[idx].id,
            dataRekam[idx].nama,
            dataRekam[idx].keluhan);
    }
    pauseScreen();
}

void cariRekamByNama() {
    clearScreen();
    loadRekam();

    char key[50];
    inputString("Masukkan nama: ", key, sizeof(key));

    int found = 0;
    for (int i = 0; i < jumlahRekam; i++) {
        if (strstr(dataRekam[i].nama, key)) {
            printf("ID:%d | %s | Keluhan:%s\n",
                dataRekam[i].id,
                dataRekam[i].nama,
                dataRekam[i].keluhan);
            found = 1;
        }
    }

    if (!found)
        printf("Data tidak ditemukan.\n");

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

/* =======================
   EDIT DATA
   ======================= */
void editRekam() {
    clearScreen();
    loadRekam();

    int id = inputInt("Masukkan ID: ");
    int idx = cariIndexById(id);

    if (idx == -1) {
        printf("ID tidak ditemukan!\n");
        pauseScreen();
        return;
    }

    inputString("Nama baru    : ", dataRekam[idx].nama, sizeof(dataRekam[idx].nama));
    inputString("Keluhan baru : ", dataRekam[idx].keluhan, sizeof(dataRekam[idx].keluhan));

    saveRekam();
    printf("Data berhasil diupdate!\n");
    pauseScreen();
}

/* =======================
   HAPUS DATA
   ======================= */
void hapusRekam() {
    clearScreen();
    loadRekam();

    int id = inputInt("Masukkan ID: ");
    int idx = cariIndexById(id);

    if (idx == -1) {
        printf("ID tidak ditemukan!\n");
        pauseScreen();
        return;
    }

    for (int i = idx; i < jumlahRekam - 1; i++) {
        dataRekam[i] = dataRekam[i + 1];
    }
    jumlahRekam--;

    saveRekam();
    printf("Data berhasil dihapus!\n");
    pauseScreen();
}

/* =======================
   MENU UTAMA
   ======================= */
void menuRekam() {
    int p;
    do {
        clearScreen();
        printf("=== MENU REKAM MEDIS ===\n");
        printf("1. Tambah Data\n");
        printf("2. Lihat Data\n");
        printf("3. Edit Data\n");
        printf("4. Hapus Data\n");
        printf("0. Kembali\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahRekam();
        else if (p == 2) lihatRekam();
        else if (p == 3) editRekam();
        else if (p == 4) hapusRekam();

    } while (p != 0);
}