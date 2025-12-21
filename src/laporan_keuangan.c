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
   CARI BY ID
   ======================= */
void tambahKeuangan() {
    clearScreen();

    printf("========================================\n");
    printf("       TAMBAH LAPORAN KEUANGAN            \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID : ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    Keuangan k;

    // cek ID
    if (f) {
        while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                      &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {
            if (k.id == id) {
                printf("\n[!] ID sudah terdaftar!\n");
                fclose(f);
                pauseScreen();
                return;
            }
        }
        fclose(f);
    }

    f = fopen(FILE_KEUANGAN, "a");
    if (!f) {
        printf("\n[!] Gagal membuka file keuangan.\n");
        pauseScreen();
        return;
    }

    k.id = id;

    printf("\n=========== INPUT DATA ===========\n");
    inputString("Jenis (MASUK/KELUAR) : ", k.jenis, sizeof(k.jenis));
    k.nominal = inputInt("Nominal             : Rp ");
    inputString("Tanggal (dd-mm-yyyy): ", k.tanggal, sizeof(k.tanggal));
    inputString("Keterangan           : ", k.keterangan, sizeof(k.keterangan));
    printf("==================================\n");

    fprintf(f, "%d|%s|%d|%s|%s\n",
            k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);

    fclose(f);

    printf("\nData keuangan berhasil ditambahkan!\n");
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
//By id
void cariKeuanganById() {
    clearScreen();

    printf("========================================\n");
    printf("      CARI DATA KEUANGAN BERDASARKAN ID  \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID Keuangan : ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    if (!f) {
        printf("\nBelum ada data keuangan.\n");
        pauseScreen();
        return;
    }

    Keuangan k;
    int found = 0;

    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {

        if (k.id == id) {
            printf("\n========== DATA KEUANGAN DITEMUKAN ==========\n");
            printf("ID        : %d\n", k.id);
            printf("Jenis     : %s\n", k.jenis);
            printf("Nominal   : Rp %d\n", k.nominal);
            printf("Tanggal   : %s\n", k.tanggal);
            printf("Keterangan: %s\n", k.keterangan);
            printf("============================================\n");
            found = 1;
            break;
        }
    }

    fclose(f);

    if (!found) {
        printf("\n[!] Data keuangan dengan ID %d tidak ditemukan.\n", id);
    }

    pauseScreen();
}
void tampilkanSemuaKeuangan() {
    clearScreen();

    FILE *f = fopen(FILE_KEUANGAN, "r");
    if (!f) {
        printf("========================================\n");
        printf("        DATA LAPORAN KEUANGAN            \n");
        printf("========================================\n");
        printf("\nBelum ada data keuangan.\n");
        printf("========================================\n");
        pauseScreen();
        return;
    }

    Keuangan k;
    int totalMasuk = 0, totalKeluar = 0;

    printf("=============================================\n");
    printf("            DATA LAPORAN KEUANGAN            \n");
    printf("=============================================\n");
    printf("| %-4s | %-7s | %-12s | %-12s | %-12s |\n",
           "ID", "Jenis", "Nominal", "Tanggal", "Keterangan");
    printf("========================================\n");

    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {

        printf("| %-4d | %-7s | Rp%-9d | %-12s | %-12s |\n",
               k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);

        if (strcmp(k.jenis, "MASUK") == 0)
            totalMasuk += k.nominal;
        else if (strcmp(k.jenis, "KELUAR") == 0)
            totalKeluar += k.nominal;
    }

    fclose(f);

    pauseScreen();
}
/* =======================
   SUBMENU LIHAT
   ======================= */
void lihatKeuangan() {
    int p;
    do {
        clearScreen();
printf("|==========================|\n");
printf("|       Inventory          |\n");
printf("|==========================|\n");
printf("|1.Lihat Semua             |\n");
printf("|--------------------------|\n");
printf("|2.Cari Keuangan By Id     |\n");
printf("|--------------------------|\n");
printf("|3.Cari Keuangan By Tanggal|\n");
printf("|--------------------------|\n");
printf("|0.Keluar                  |\n");
printf("|--------------------------|\n");

        p = inputInt("Pilih: ");

        switch (p) {
            case 1: tampilkanSemuaKeuangan(); break;
            case 2: cariKeuanganById(); break;
            case 3: cariKeuanganByTanggal(); break;
        }
    } while (p != 0);
}

/* =======================
   EDIT DATA
   ======================= */
void editKeuangan() {
    clearScreen();

    printf("========================================\n");
    printf("          EDIT DATA KEUANGAN              \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID Keuangan : ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Keuangan k;
    int found = 0;

    if (!f || !tmp) {
        printf("\n[!] Gagal membuka file keuangan.\n");
        pauseScreen();
        return;
    }

    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {

        if (k.id == id) {
            found = 1;

            printf("\nData Lama\n");
            printf("----------------------------------------\n");
            printf("Jenis      : %s\n", k.jenis);
            printf("Nominal    : Rp%d\n", k.nominal);
            printf("Tanggal    : %s\n", k.tanggal);
            printf("Keterangan : %s\n", k.keterangan);
            printf("----------------------------------------\n");

            printf("\nMasukkan Data Baru\n");
            inputString("Jenis (MASUK/KELUAR) : ", k.jenis, sizeof(k.jenis));
            k.nominal = inputInt("Nominal             : ");
            inputString("Tanggal (dd-mm-yyyy): ", k.tanggal, sizeof(k.tanggal));
            inputString("Keterangan          : ", k.keterangan, sizeof(k.keterangan));
        }

        fprintf(tmp, "%d|%s|%d|%s|%s\n",
                k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);
    }

    fclose(f);
    fclose(tmp);
    remove(FILE_KEUANGAN);
    rename(FILE_TEMP, FILE_KEUANGAN);

    if (found)
        printf("\n[✓] Data keuangan berhasil diperbarui!\n");
    else
        printf("\n[!] ID tidak ditemukan.\n");

    pauseScreen();
}

/* =======================
   HAPUS DATA
   ======================= */
void hapusKeuangan() {
    clearScreen();

    printf("========================================\n");
    printf("          HAPUS DATA KEUANGAN            \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID Keuangan : ");

    FILE *f = fopen(FILE_KEUANGAN, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Keuangan k;
    Keuangan dataHapus;
    int found = 0;

    if (!f || !tmp) {
        printf("\n[!] Gagal membuka file keuangan.\n");
        pauseScreen();
        return;
    }

    while (fscanf(f, "%d|%19[^|]|%d|%19[^|]|%99[^\n]\n",
                  &k.id, k.jenis, &k.nominal, k.tanggal, k.keterangan) != EOF) {

        if (k.id == id) {
            found = 1;
            dataHapus = k;   
            continue;
        }

        fprintf(tmp, "%d|%s|%d|%s|%s\n",
                k.id, k.jenis, k.nominal, k.tanggal, k.keterangan);
    }

    fclose(f);
    fclose(tmp);
    remove(FILE_KEUANGAN);
    rename(FILE_TEMP, FILE_KEUANGAN);

    if (found) {
        printf("\n========== DATA YANG DIHAPUS ==========\n");
        printf("ID        : %d\n", dataHapus.id);
        printf("Jenis     : %s\n", dataHapus.jenis);
        printf("Nominal   : Rp %d\n", dataHapus.nominal);
        printf("Tanggal   : %s\n", dataHapus.tanggal);
        printf("Keterangan: %s\n", dataHapus.keterangan);
        printf("======================================\n");
        printf("\nData keuangan berhasil dihapus!\n");
    } else {
        printf("\nID tidak ditemukan.\n");
    }

    pauseScreen();
}

/* =======================
   MENU UTAMA
   ======================= */
void menuKeuangan() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|        Keuangan         |\n");
printf("|=========================|\n");
printf("|1.Tambah Data Keuangan   |\n");
printf("|-------------------------|\n");
printf("|2.Lihat Data Keuangan    |\n");
printf("|-------------------------|\n");
printf("|3.Edit Data Keuangan     |\n");
printf("|-------------------------|\n");
printf("|4.Hapus Data             |\n");
printf("|-------------------------|\n");
printf("|0.Keluar                 |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahKeuangan();
        else if (p == 2) lihatKeuangan();
        else if (p == 3) editKeuangan();
        else if (p == 4) hapusKeuangan();

    } while (p != 0);
}
