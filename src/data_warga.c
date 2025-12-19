#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_warga.h"
#include "utils.h"

#define FILE_WARGA "db/data_warga.txt"

//ARRAY OF STRUCT
Warga warga[MAX_WARGA];
int jumlahWarga = 0;

/* ===== LOAD ===== */
void loadDataWarga() {
    FILE *f = fopen(FILE_WARGA, "r");
    jumlahWarga = 0;

    if (!f) return;

    while (jumlahWarga < MAX_WARGA &&
           fscanf(f, "%d|%49[^|]|%99[^|]|%d\n",
                  &warga[jumlahWarga].id,
                  warga[jumlahWarga].nama,
                  warga[jumlahWarga].alamat,
                  &warga[jumlahWarga].umur) == 4) {
        jumlahWarga++;
    }
    fclose(f);
}

/* ===== SAVE ===== */
void saveDataWarga() {
    FILE *f = fopen(FILE_WARGA, "w");
    if (!f) return;

    for (int i = 0; i < jumlahWarga; i++) {
        fprintf(f, "%d|%s|%s|%d\n",
                warga[i].id,
                warga[i].nama,
                warga[i].alamat,
                warga[i].umur);
    }
    fclose(f);
}

/* ===== TAMBAH ===== */
void tambahWarga() {
    clearScreen();
    printf("=== TAMBAH DATA WARGA ===\n");

    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    for (int i = 0; i < jumlahWarga; i++) {
        if (warga[i].id == id) {
            printf("ID sudah terdaftar!\n");
            pauseScreen();
            return;
        }
    }

    Warga w;
    w.id = id;
    inputString("Nama   : ", w.nama, sizeof(w.nama));
    inputString("Alamat : ", w.alamat, sizeof(w.alamat));
    w.umur = inputInt("Umur   : ");

    w.nama[strcspn(w.nama, "\n")] = 0;
    w.alamat[strcspn(w.alamat, "\n")] = 0;

    warga[jumlahWarga++] = w;
    saveDataWarga();

    printf("Data berhasil ditambahkan!\n");
    pauseScreen();
}


void tampilkanSemuaWarga() {
    clearScreen();

    if (jumlahWarga == 0) {
        printf("Belum ada data.\n");
        pauseScreen();
        return;
    }

//Bubble Sort
    for (int i = 0; i < jumlahWarga - 1; i++) {
        for (int j = 0; j < jumlahWarga - i - 1; j++) {
            if (strcmp(warga[j].nama, warga[j + 1].nama) > 0) {
                Warga tmp = warga[j];
                warga[j] = warga[j + 1];
                warga[j + 1] = tmp;
            }
        }
    }

    printf("=== DATA WARGA ===\n");
    for (int i = 0; i < jumlahWarga; i++) {
        printf("ID:%d | Nama:%s | Alamat:%s | Umur:%d\n",
               warga[i].id,
               warga[i].nama,
               warga[i].alamat,
               warga[i].umur);
    }
    pauseScreen();
}

/* ===== SEARCH ===== */
void cariWargaById() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");
//Sequential Search
    for (int i = 0; i < jumlahWarga; i++) {
        if (warga[i].id == id) {
            printf("ID:%d\nNama:%s\nAlamat:%s\nUmur:%d\n",
                   warga[i].id,
                   warga[i].nama,
                   warga[i].alamat,
                   warga[i].umur);
            pauseScreen();
            return;
        }
    }
    printf("Data tidak ditemukan.\n");
    pauseScreen();
}

void cariWargaByNama() {
    clearScreen();
    char key[50];
    inputString("Nama: ", key, sizeof(key));
    key[strcspn(key, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < jumlahWarga; i++) {
        if (strstr(warga[i].nama, key)) {
            printf("ID:%d | Nama:%s | Alamat:%s | Umur:%d\n",
                   warga[i].id,
                   warga[i].nama,
                   warga[i].alamat,
                   warga[i].umur);
            found = 1;
        }
    }
    if (!found) printf("Tidak ditemukan.\n");
    pauseScreen();
}

/* ===== EDIT ===== */
void editWarga() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");

    for (int i = 0; i < jumlahWarga; i++) {
        if (warga[i].id == id) {
            inputString("Nama baru   : ", warga[i].nama, sizeof(warga[i].nama));
            inputString("Alamat baru : ", warga[i].alamat, sizeof(warga[i].alamat));
            warga[i].umur = inputInt("Umur baru   : ");

            warga[i].nama[strcspn(warga[i].nama, "\n")] = 0;
            warga[i].alamat[strcspn(warga[i].alamat, "\n")] = 0;

            saveDataWarga();
            printf("Data diperbarui!\n");
            pauseScreen();
            return;
        }
    }
    printf("ID tidak ditemukan.\n");
    pauseScreen();
}

/* ===== HAPUS ===== */
void hapusWarga() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");

    for (int i = 0; i < jumlahWarga; i++) {
        if (warga[i].id == id) {
            for (int j = i; j < jumlahWarga - 1; j++) {
                warga[j] = warga[j + 1];
            }
            jumlahWarga--;
            saveDataWarga();
            printf("Data dihapus!\n");
            pauseScreen();
            return;
        }
    }
    printf("ID tidak ditemukan.\n");
    pauseScreen();
}

/* ===== MENU ===== */
void lihatWarga() {
    int p;
    do {
        clearScreen();
        printf("1. Tampilkan Semua\n");
        printf("2. Cari ID\n");
        printf("3. Cari Nama\n");
        printf("0. Kembali\n");
        p = inputInt("Pilih: ");

        if (p == 1) tampilkanSemuaWarga();
        else if (p == 2) cariWargaById();
        else if (p == 3) cariWargaByNama();
    } while (p != 0);
}

void menuWarga() {
    loadDataWarga();

    int p;
    do {
        clearScreen();
        printf("=== DATA WARGA ===\n");
        printf("1. Tambah\n");
        printf("2. Lihat\n");
        printf("3. Edit\n");
        printf("4. Hapus\n");
        printf("0. Kembali\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahWarga();
        else if (p == 2) lihatWarga();
        else if (p == 3) editWarga();
        else if (p == 4) hapusWarga();
    } while (p != 0);
}
