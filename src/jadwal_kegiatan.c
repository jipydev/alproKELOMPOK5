#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jadwal_kegiatan.h"
#include "utils.h"

#define FILE_JADWAL "db/jadwal_kegiatan.txt"
#define FILE_TEMP   "db/temp.txt"
#define MAX_DATA    100

/* =====================================================
   FUNGSI UTIL INTERNAL (LOAD & SORT)
   ===================================================== */

/* Membaca seluruh data jadwal ke array */
int loadJadwal(Jadwal data[], int max) {
    FILE *f = fopen(FILE_JADWAL, "r");
    if (!f) return 0;

    int n = 0;
    while (n < max &&
           fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                  &data[n].id, data[n].kegiatan, data[n].tanggal) != EOF) {
        n++;
    }
    fclose(f);
    return n;
}

/* Bubble Sort berdasarkan tanggal */
void sortJadwalByTanggal(Jadwal data[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(data[j].tanggal, data[j + 1].tanggal) > 0) {
                Jadwal temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

/* =====================================================
   TAMBAH DATA
   ===================================================== */
void tambahJadwal() {
    clearScreen();
    printf("=== TAMBAH JADWAL KEGIATAN ===\n");

    int id = inputInt("Masukkan ID Jadwal (0 = batal): ");
    if (id == 0) return;

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);

    /* Cegah ID duplikat */
    for (int i = 0; i < n; i++) {
        if (data[i].id == id) {
            printf("ID sudah ada!\n");
            pauseScreen();
            return;
        }
    }

    FILE *f = fopen(FILE_JADWAL, "a");
    if (!f) return;

    Jadwal j;
    j.id = id;
    inputString("Nama kegiatan: ", j.kegiatan, sizeof(j.kegiatan));
    inputString("Tanggal (contoh: 22-12-2025): ", j.tanggal, sizeof(j.tanggal));

    fprintf(f, "%d|%s|%s\n", j.id, j.kegiatan, j.tanggal);
    fclose(f);

    printf("Jadwal berhasil ditambahkan!\n");
    pauseScreen();
}

/* =====================================================
   TAMPILKAN DATA
   ===================================================== */
void tampilkanSemuaJadwal() {
    clearScreen();

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);

    if (n == 0) {
        printf("Belum ada data jadwal.\n");
        pauseScreen();
        return;
    }

    printf("=== DAFTAR JADWAL KEGIATAN ===\n");
    for (int i = 0; i < n; i++) {
        printf("ID:%d | %s | %s\n",
               data[i].id, data[i].kegiatan, data[i].tanggal);
    }
    pauseScreen();
}

void tampilkanJadwalTerurut() {
    clearScreen();

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);

    if (n == 0) {
        printf("Belum ada data jadwal.\n");
        pauseScreen();
        return;
    }

    sortJadwalByTanggal(data, n);

    printf("=== JADWAL TERURUT BERDASARKAN TANGGAL ===\n");
    for (int i = 0; i < n; i++) {
        printf("ID:%d | %s | %s\n",
               data[i].id, data[i].kegiatan, data[i].tanggal);
    }
    pauseScreen();
}

/* =====================================================
   SEARCHING
   ===================================================== */
void cariJadwalById() {
    clearScreen();
    int id = inputInt("Masukkan ID (0 = kembali): ");
    if (id == 0) return;

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);

    for (int i = 0; i < n; i++) {
        if (data[i].id == id) {
            printf("ID:%d\nKegiatan:%s\nTanggal:%s\n",
                   data[i].id, data[i].kegiatan, data[i].tanggal);
            pauseScreen();
            return;
        }
    }

    printf("Jadwal tidak ditemukan.\n");
    pauseScreen();
}

void cariJadwalByNama() {
    clearScreen();
    char key[50];
    inputString("Masukkan nama kegiatan: ", key, sizeof(key));

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);
    int found = 0;

    for (int i = 0; i < n; i++) {
        if (strstr(data[i].kegiatan, key)) {
            printf("ID:%d | %s | %s\n",
                   data[i].id, data[i].kegiatan, data[i].tanggal);
            found = 1;
        }
    }

    if (!found) printf("Tidak ada jadwal ditemukan.\n");
    pauseScreen();
}

void cariJadwalByTanggal() {
    clearScreen();
    char key[30];
    inputString("Masukkan tanggal: ", key, sizeof(key));

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);
    int found = 0;

    printf("\n=== JADWAL TANGGAL %s ===\n", key);
    for (int i = 0; i < n; i++) {
        if (strcmp(data[i].tanggal, key) == 0) {
            printf("ID:%d | %s\n",
                   data[i].id, data[i].kegiatan);
            found = 1;
        }
    }

    if (!found) printf("Tidak ada jadwal.\n");
    pauseScreen();
}

/* =====================================================
   SUBMENU LIHAT
   ===================================================== */
void lihatJadwal() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|       Inventory         |\n");
printf("|=========================|\n");
printf("|1.Tampilkan Semua Jadwal |\n");
printf("|-------------------------|\n");
printf("|2.Cari Jadwal Terurut    |\n");
printf("|-------------------------|\n");
printf("|3.Cari Jadwal By Id      |\n");
printf("|-------------------------|\n");
printf("|4.Cari Jadwal By Nama    |\n");
printf("|-------------------------|\n");
printf("|5.Cari Jadwal By Tanggal |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tampilkanSemuaJadwal();
        else if (p == 2) tampilkanJadwalTerurut();
        else if (p == 3) cariJadwalById();
        else if (p == 4) cariJadwalByNama();
        else if (p == 5) cariJadwalByTanggal();

    } while (p != 0);
}

/* =====================================================
   EDIT & HAPUS
   ===================================================== */
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

/* =====================================================
   MENU UTAMA
   ===================================================== */
void menuJadwal() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|         Jadwal          |\n");
printf("|=========================|\n");
printf("|1.Tambah Jadwal          |\n");
printf("|-------------------------|\n");
printf("|2.Lihat Jadwal           |\n");
printf("|-------------------------|\n");
printf("|3.Edit Jadwal            |\n");
printf("|-------------------------|\n");
printf("|4.Hapus Jadwal           |\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahJadwal();
        else if (p == 2) lihatJadwal();
        else if (p == 3) editJadwal();
        else if (p == 4) hapusJadwal();

    } while (p != 0);
}
