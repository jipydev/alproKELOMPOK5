#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "laporan_keuangan.h"
#include "utils.h"

#define FILE_KEUANGAN "db/laporan_keuangan.txt"
#define FILE_TEMP     "db/tmp.txt"
#define MAX_DATA      100

typedef struct {
    int id;
    char jenis[20];
    int nominal;
    char tanggal[20];
    char keterangan[100];
} Keuangan;

/* =====================================================
   LOAD DATA
   ===================================================== */
int loadKeuangan(Keuangan data[], int max) {
    FILE *f = fopen(FILE_KEUANGAN, "r");
    if (!f) return 0;

    int n = 0;
    while (n < max &&
           fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &data[n].id, data[n].jenis,
                  &data[n].nominal, data[n].tanggal,
                  data[n].keterangan) != EOF) {
        n++;
    }
    fclose(f);
    return n;
}

/* =====================================================
   SORTING (BUBBLE SORT BY ID)
   ===================================================== */
void sortKeuanganById(Keuangan data[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j].id > data[j + 1].id) {
                Keuangan tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
        }
    }
}

/* =====================================================
   TAMBAH DATA
   ===================================================== */
void tambahKeuangan() {
    clearScreen();
    printf("=== TAMBAH LAPORAN KEUANGAN ===\n");

    int id = inputInt("Masukkan ID (0 = batal): ");
    if (id == 0) return;

    Keuangan data[MAX_DATA];
    int n = loadKeuangan(data, MAX_DATA);

    for (int i = 0; i < n; i++) {
        if (data[i].id == id) {
            printf("ID sudah terdaftar!\n");
            pauseScreen();
            return;
        }
    }

    FILE *f = fopen(FILE_KEUANGAN, "a");
    if (!f) return;

    Keuangan k;
    k.id = id;
    inputString("Jenis (MASUK/KELUAR): ", k.jenis, sizeof(k.jenis));
    k.nominal = inputInt("Nominal: Rp ");
    inputString("Tanggal (dd-mm-yyyy): ", k.tanggal, sizeof(k.tanggal));
    inputString("Keterangan: ", k.keterangan, sizeof(k.keterangan));

    fprintf(f, "%d|%s|%d|%s|%s\n",
            k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);

    fclose(f);
    printf("Data keuangan berhasil ditambahkan!\n");
    pauseScreen();
}

/* =====================================================
   TAMPILKAN SEMUA (TERURUT ID)
   ===================================================== */
void tampilkanSemuaKeuangan() {
    clearScreen();

    Keuangan data[MAX_DATA];
    int n = loadKeuangan(data, MAX_DATA);

    if (n == 0) {
        printf("Belum ada data keuangan.\n");
        pauseScreen();
        return;
    }

    sortKeuanganById(data, n);

    printf("=== DATA LAPORAN KEUANGAN (URUT ID) ===\n");
    printf("| ID | Jenis  | Nominal | Tanggal | Keterangan |\n");
    printf("----------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("| %d | %s | Rp%d | %s | %s |\n",
               data[i].id,
               data[i].jenis,
               data[i].nominal,
               data[i].tanggal,
               data[i].keterangan);
    }

    pauseScreen();
}

/* =====================================================
   SEARCHING (SEQUENTIAL SEARCH)
   ===================================================== */
void cariKeuanganById() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");

    Keuangan data[MAX_DATA];
    int n = loadKeuangan(data, MAX_DATA);

    for (int i = 0; i < n; i++) {
        if (data[i].id == id) {
            printf("ID        : %d\n", data[i].id);
            printf("Jenis     : %s\n", data[i].jenis);
            printf("Nominal   : Rp %d\n", data[i].nominal);
            printf("Tanggal   : %s\n", data[i].tanggal);
            printf("Keterangan: %s\n", data[i].keterangan);
            pauseScreen();
            return;
        }
    }

    printf("Data tidak ditemukan.\n");
    pauseScreen();
}

void cariKeuanganByTanggal() {
    clearScreen();
    char key[20];
    inputString("Masukkan tanggal: ", key, sizeof(key));

    Keuangan data[MAX_DATA];
    int n = loadKeuangan(data, MAX_DATA);
    int found = 0;

    printf("\n=== DATA TANGGAL %s ===\n", key);
    for (int i = 0; i < n; i++) {
        if (strcmp(data[i].tanggal, key) == 0) {
            printf("ID:%d | %s | Rp%d | %s\n",
                   data[i].id,
                   data[i].jenis,
                   data[i].nominal,
                   data[i].keterangan);
            found = 1;
        }
    }

    if (!found)
        printf("Tidak ada data.\n");

    pauseScreen();
}

/* =====================================================
   EDIT DATA
   ===================================================== */
void editKeuangan() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Keuangan k;
    int found = 0;

    while (f && fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                       &k.id, k.jenis, &k.nominal,
                       k.tanggal, k.keterangan) != EOF) {

        if (k.id == id) {
            found = 1;
            inputString("Jenis baru: ", k.jenis, sizeof(k.jenis));
            k.nominal = inputInt("Nominal baru: Rp ");
            inputString("Tanggal baru: ", k.tanggal, sizeof(k.tanggal));
            inputString("Keterangan baru: ", k.keterangan, sizeof(k.keterangan));
        }

        fprintf(tmp, "%d|%s|%d|%s|%s\n",
                k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);
    }

    if (f) fclose(f);
    fclose(tmp);
    remove(FILE_KEUANGAN);
    rename(FILE_TEMP, FILE_KEUANGAN);

    printf(found ? "Data diperbarui!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* =====================================================
   HAPUS DATA
   ===================================================== */
void hapusKeuangan() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Keuangan k;
    int found = 0;

    while (f && fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                       &k.id, k.jenis, &k.nominal,
                       k.tanggal, k.keterangan) != EOF) {

        if (k.id == id) {
            found = 1;
            continue;
        }
        fprintf(tmp, "%d|%s|%d|%s|%s\n",
                k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);
    }

    if (f) fclose(f);
    fclose(tmp);
    remove(FILE_KEUANGAN);
    rename(FILE_TEMP, FILE_KEUANGAN);

    printf(found ? "Data dihapus!\n" : "ID tidak ditemukan!\n");
    pauseScreen();
}

/* =====================================================
   MENU
   ===================================================== */
void menuKeuangan() {
    int p;
    do {
        clearScreen();
        printf("=== MENU LAPORAN KEUANGAN ===\n");
        printf("1. Tambah\n");
        printf("2. Lihat\n");
        printf("3. Edit\n");
        printf("4. Hapus\n");
        printf("0. Kembali\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahKeuangan();
        else if (p == 2) {
            int x;
            do {
                clearScreen();
                printf("1. Tampilkan Semua\n");
                printf("2. Cari by ID\n");
                printf("3. Cari by Tanggal\n");
                printf("0. Kembali\n");

                x = inputInt("Pilih: ");
                if (x == 1) tampilkanSemuaKeuangan();
                else if (x == 2) cariKeuanganById();
                else if (x == 3) cariKeuanganByTanggal();
            } while (x != 0);
        }
        else if (p == 3) editKeuangan();
        else if (p == 4) hapusKeuangan();

    } while (p != 0);
}
