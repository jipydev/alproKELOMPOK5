#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "laporan_keuangan.h"
#include "utils.h"

#define FILE_KEUANGAN "db/laporan_keuangan.txt"
#define FILE_TEMP     "db/tmp.txt"

typedef struct {
    int id;
    char jenis[20];        
    int nominal;
    char tanggal[20];    
    char keterangan[100];
} Keuangan;

/* =======================
   TAMBAH DATA
   ======================= */
void tambahKeuangan() {
    clearScreen();
    printf("=== TAMBAH LAPORAN KEUANGAN ===\n");

    int id = inputInt("Masukkan ID: ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    Keuangan k;

    // cek ID duplikat
    if (f) {
        while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                      &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {
            if (k.id == id) {
                printf("ID sudah ada!\n");
                fclose(f);
                pauseScreen();
                return;
            }
        }
        fclose(f);
    }

    f = fopen(FILE_KEUANGAN, "a");
    if (!f) {
        printf("Gagal membuka file.\n");
        pauseScreen();
        return;
    }

    k.id = id;
    inputString("Jenis (MASUK/KELUAR): ", k.jenis, sizeof(k.jenis));
    k.nominal = inputInt("Nominal: ");
    inputString("Tanggal (contoh: 12-09-2025): ", k.tanggal, sizeof(k.tanggal));
    inputString("Keterangan: ", k.keterangan, sizeof(k.keterangan));

    fprintf(f, "%d|%s|%d|%s|%s\n",
            k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);

    fclose(f);
    printf("Data keuangan berhasil ditambahkan!\n");
    pauseScreen();
}

/* =======================
   TAMPILKAN SEMUA
   ======================= */
void tampilkanSemuaKeuangan() {
    clearScreen();
    FILE *f = fopen(FILE_KEUANGAN, "r");
    if (!f) {
        printf("Belum ada data.\n");
        pauseScreen();
        return;
    }

    Keuangan k;
    printf("=== DATA LAPORAN KEUANGAN ===\n");
    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {
        printf("ID:%d | %s | Rp%d | %s\nKet: %s\n\n",
               k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);
    }

    fclose(f);
    pauseScreen();
}

/* =======================
   CARI BY ID
   ======================= */
void cariKeuanganById() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    if (!f) {
        printf("Belum ada data.\n");
        pauseScreen();
        return;
    }

    Keuangan k;
    int found = 0;

    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {
        if (k.id == id) {
            printf("ID:%d\nJenis:%s\nNominal:%d\nTanggal:%s\nKet:%s\n",
                   k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);
            found = 1;
            break;
        }
    }

    fclose(f);
    if (!found) printf("ID tidak ditemukan.\n");
    pauseScreen();
}

/* =======================
   CARI BY JENIS
   ======================= */
void cariKeuanganByJenis() {
    clearScreen();
    char cari[20];
    inputString("Masukkan jenis (MASUK/KELUAR): ", cari, sizeof(cari));

    FILE *f = fopen(FILE_KEUANGAN, "r");
    if (!f) {
        printf("Belum ada data.\n");
        pauseScreen();
        return;
    }

    Keuangan k;
    int found = 0;

    printf("\nHASIL PENCARIAN:\n");
    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {
        if (strcmp(k.jenis, cari) == 0) {
            printf("ID:%d | Rp%d | %s | %s\n",
                   k.id, k.nominal, k.tanggal, k.keterangan);
            found = 1;
        }
    }

    fclose(f);
    if (!found) printf("Data tidak ditemukan.\n");
    pauseScreen();
}

/* =======================
   CARI BY TANGGAL
   ======================= */
void cariKeuanganByTanggal() {
    clearScreen();
    char cari[20];
    inputString("Masukkan tanggal : ", cari, sizeof(cari));

    FILE *f = fopen(FILE_KEUANGAN, "r");
    if (!f) {
        printf("Belum ada data.\n");
        pauseScreen();
        return;
    }

    Keuangan k;
    int found = 0;

    printf("\nHASIL PENCARIAN:\n");
    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {
        if (strcmp(k.tanggal, cari) == 0) {
            printf("ID:%d | %s | Rp%d | %s\n",
                   k.id, k.jenis, k.nominal, k.keterangan);
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
void lihatKeuangan() {
    int p;
    do {
        clearScreen();
        printf("=== LIHAT LAPORAN KEUANGAN ===\n");
        printf("1. Tampilkan Semua\n");
        printf("2. Cari berdasarkan ID\n");
        printf("3. Cari berdasarkan Jenis\n");
        printf("4. Cari berdasarkan Tanggal\n");
        printf("0. Kembali\n");

        p = inputInt("Pilih: ");

        switch (p) {
            case 1: tampilkanSemuaKeuangan(); break;
            case 2: cariKeuanganById(); break;
            case 3: cariKeuanganByJenis(); break;
            case 4: cariKeuanganByTanggal(); break;
        }
    } while (p != 0);
}

/* =======================
   EDIT DATA
   ======================= */
void editKeuangan() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Keuangan k;
    int found = 0;

    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {
        if (k.id == id) {
            found = 1;
            inputString("Jenis baru: ", k.jenis, sizeof(k.jenis));
            k.nominal = inputInt("Nominal baru: ");
            inputString("Tanggal baru: ", k.tanggal, sizeof(k.tanggal));
            inputString("Keterangan baru: ", k.keterangan, sizeof(k.keterangan));
        }
        fprintf(tmp, "%d|%s|%d|%s|%s\n",
                k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);
    }

    fclose(f); fclose(tmp);
    remove(FILE_KEUANGAN);
    rename(FILE_TEMP, FILE_KEUANGAN);

    if (found) printf("Data berhasil diubah!\n");
    else printf("ID tidak ditemukan.\n");

    pauseScreen();
}

/* =======================
   HAPUS DATA
   ======================= */
void hapusKeuangan() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Keuangan k;
    int found = 0;

    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {
        if (k.id == id) {
            found = 1;
            continue;
        }
        fprintf(tmp, "%d|%s|%d|%s|%s\n",
                k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);
    }

    fclose(f); fclose(tmp);
    remove(FILE_KEUANGAN);
    rename(FILE_TEMP, FILE_KEUANGAN);

    if (found) printf("Data berhasil dihapus!\n");
    else printf("ID tidak ditemukan.\n");

    pauseScreen();
}

/* =======================
   MENU UTAMA
   ======================= */
void menuKeuangan() {
    int p;
    do {
        clearScreen();
        printf("=== MENU LAPORAN KEUANGAN ===\n");
        printf("1. Tambah Data\n");
        printf("2. Lihat Data\n");
        printf("3. Edit Data\n");
        printf("4. Hapus Data\n");
        printf("0. Kembali\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahKeuangan();
        else if (p == 2) lihatKeuangan();
        else if (p == 3) editKeuangan();
        else if (p == 4) hapusKeuangan();

    } while (p != 0);
}
