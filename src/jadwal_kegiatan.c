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
/* Bubble Sort berdasarkan ID */
void sortJadwalById(Jadwal data[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (data[j].id > data[j + 1].id) {
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

    printf("========================================\n");
    printf("        TAMBAH JADWAL KEGIATAN            \n");
    printf("========================================\n\n");

    int id = inputInt("Masukkan ID Jadwal (0 = batal) : ");
    if (id == 0) {
        printf("\n========================================\n");
        printf("Proses penambahan dibatalkan.\n");
        printf("========================================\n");
        pauseScreen();
        return;
    }

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);

    /* Cegah ID duplikat */
    for (int i = 0; i < n; i++) {
        if (data[i].id == id) {
            printf("\n========================================\n");
            printf("ID Jadwal sudah terdaftar!\n");
            printf("========================================\n");
            pauseScreen();
            return;
        }
    }

    FILE *f = fopen(FILE_JADWAL, "a");
    if (!f) {
        printf("\nGagal membuka file jadwal!\n");
        pauseScreen();
        return;
    }

    Jadwal j;
    j.id = id;

    printf("\n=========== INPUT DATA JADWAL ===========\n");
    inputString("Nama Kegiatan  : ", j.kegiatan, sizeof(j.kegiatan));
    inputString("Tanggal (dd-mm-yyyy): ", j.tanggal, sizeof(j.tanggal));
    printf("========================================\n");

    fprintf(f, "%d|%s|%s\n", j.id, j.kegiatan, j.tanggal);
    fclose(f);

    printf("\nJadwal berhasil ditambahkan!\n");
    pauseScreen();
}

/* =====================================================
   TAMPILKAN DATA
   ===================================================== */
void tampilkanSemuaJadwal() {
    clearScreen();

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);

    printf("========================================\n");
    printf("        DAFTAR JADWAL KEGIATAN            \n");
    printf("========================================\n");

    if (n == 0) {
        printf("\nBelum ada data jadwal.\n");
        printf("========================================\n");
        pauseScreen();
        return;
    }

    sortJadwalById(data, n);

    printf("=== DAFTAR JADWAL KEGIATAN (URUT ID) ===\n");

    for (int i = 0; i < n; i++) {
        printf("ID:%d | Kegiatan:%s | Tanggal:%s\n",
               data[i].id,
               data[i].kegiatan,
               data[i].tanggal);
    }

    pauseScreen();
}



/* =====================================================
   SEARCHING
   ===================================================== */
void cariJadwalById() {
    clearScreen();

    printf("========================================\n");
    printf("        CARI JADWAL BERDASARKAN ID        \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID (0 = kembali) : ");
    if (id == 0) {
        printf("\nKembali ke menu...\n");
        pauseScreen();
        return;
    }

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);

    for (int i = 0; i < n; i++) {
        if (data[i].id == id) {
            printf("\n========== JADWAL DITEMUKAN ==========\n");
            printf("ID       : %d\n", data[i].id);
            printf("Kegiatan : %s\n", data[i].kegiatan);
            printf("Tanggal  : %s\n", data[i].tanggal);
            printf("=====================================\n");
            pauseScreen();
            return;
        }
    }

    printf("\nJadwal dengan ID %d tidak ditemukan.\n", id);
    pauseScreen();
}


void cariJadwalByNama() {
    clearScreen();

    printf("========================================\n");
    printf("     CARI JADWAL BERDASARKAN KEGIATAN     \n");
    printf("========================================\n");

    char key[50];
    inputString("Masukkan nama kegiatan : ", key, sizeof(key));

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);
    int found = 0;

    printf("\n| %-4s | %-25s | %-12s |\n",
           "ID", "Nama Kegiatan", "Tanggal");
    printf("========================================\n");

    for (int i = 0; i < n; i++) {
        if (strstr(data[i].kegiatan, key)) {
            printf("ID:%d | Kegiatan:%s | Tanggal:%s\n",
                   data[i].id, data[i].kegiatan, data[i].tanggal);
            found = 1;
        }
    }

    if (!found) {
        printf("\n Tidak ada jadwal dengan nama \"%s\".\n", key);
    } else {
        printf("========================================\n");
    }

    pauseScreen();
}
void cariJadwalByTanggal() {
    clearScreen();

    printf("========================================\n");
    printf("      CARI JADWAL BERDASARKAN TANGGAL     \n");
    printf("========================================    \n");

    char key[20];
    inputString("Masukkan tanggal (dd-mm-yyyy): ", key, sizeof(key));

    Jadwal data[MAX_DATA];
    int n = loadJadwal(data, MAX_DATA);
    int found = 0;

    printf("\n| %-4s | %-25s | %-12s |\n",
           "ID", "Nama Kegiatan", "Tanggal");
    printf("==============================================\n");

    for (int i = 0; i < n; i++) {
        if (strcmp(data[i].tanggal, key) == 0) {
            printf("ID:%d | Kegiatan:%s\n",
                   data[i].id, data[i].kegiatan);
            found = 1;
        }
    }

    if (!found) {
        printf("\n[!] Tidak ada jadwal pada tanggal %s.\n", key);
    } else {
        printf("==============================================\n");
    }

    pauseScreen();
}



/* =====================================================
   SUBMENU LIHAT
   ===================================================== */
void lihatJadwal() {
    int p;
    do {
        clearScreen();
        printf("|===================================|\n");
        printf("|       Lihat Jadwal Kegiatan       |\n");
        printf("|===================================|\n");
        printf("|1.Tampilkan Semua Jadwal |\n");
        printf("|-------------------------|\n");
        printf("|2.Cari Jadwal By Id      |\n");
        printf("|-------------------------|\n");
        printf("|3.Cari Jadwal By Nama    |\n");
        printf("|-------------------------|\n");
        printf("|4.Cari Jadwal By Tanggal |\n");
        printf("|-------------------------|\n");
        printf("|0.Keluar                 |\n");
        printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tampilkanSemuaJadwal();
        else if (p == 2) cariJadwalById();
        else if (p == 3) cariJadwalByNama();
        else if (p == 4) cariJadwalByTanggal();

    } while (p != 0);
}

/* =====================================================
   EDIT & HAPUS
   ===================================================== */
void editJadwal() {
    clearScreen();

    printf("========================================\n");
    printf("           EDIT JADWAL KEGIATAN           \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID Jadwal (0 = batal) : ");
    if (id == 0) {
        printf("\nProses edit dibatalkan.\n");
        pauseScreen();
        return;
    }

    FILE *f = fopen(FILE_JADWAL, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Jadwal j;
    int found = 0;

    if (!f || !tmp) {
        printf("\n[!] Gagal membuka file jadwal.\n");
        pauseScreen();
        return;
    }

    while (fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                  &j.id, j.kegiatan, j.tanggal) != EOF) {

        if (j.id == id) {
            found = 1;

            printf("\nData Lama\n");
            printf("----------------------------------------\n");
            printf("Nama Kegiatan : %s\n", j.kegiatan);
            printf("Tanggal       : %s\n", j.tanggal);
            printf("----------------------------------------\n");

            printf("\nMasukkan Data Baru\n");
            inputString("Nama Kegiatan : ", j.kegiatan, sizeof(j.kegiatan));
            inputString("Tanggal (dd-mm-yyyy): ", j.tanggal, sizeof(j.tanggal));
        }

        fprintf(tmp, "%d|%s|%s\n", j.id, j.kegiatan, j.tanggal);
    }

    fclose(f);
    fclose(tmp);
    remove(FILE_JADWAL);
    rename(FILE_TEMP, FILE_JADWAL);

    if (found)
        printf("\nJadwal berhasil diperbarui!\n");
    else
        printf("\nID tidak ditemukan.\n");

    pauseScreen();
}

void hapusJadwal() {
    clearScreen();

    printf("========================================\n");
    printf("          HAPUS JADWAL KEGIATAN           \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID Jadwal (0 = batal) : ");
    if (id == 0) {
        printf("\nProses penghapusan dibatalkan.\n");
        pauseScreen();
        return;
    }

    FILE *f = fopen(FILE_JADWAL, "r");
    FILE *tmp = fopen(FILE_TEMP, "w");
    Jadwal j;
    int found = 0;

    if (!f || !tmp) {
        printf("\nGagal membuka file jadwal.\n");
        pauseScreen();
        return;
    }

    while (fscanf(f, "%d|%49[^|]|%29[^\n]\n",
                  &j.id, j.kegiatan, j.tanggal) != EOF) {

        if (j.id == id) {
            found = 1;
            continue;
        }

        fprintf(tmp, "%d|%s|%s\n", j.id, j.kegiatan, j.tanggal);
    }

    fclose(f);
    fclose(tmp);
    remove(FILE_JADWAL);
    rename(FILE_TEMP, FILE_JADWAL);

    if (found)
        printf("\nJadwal berhasil dihapus!\n");
    else
        printf("\nID tidak ditemukan.\n");

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
printf("|-------------------------|\n");
printf("|0.Keluar                 |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahJadwal();
        else if (p == 2) lihatJadwal();
        else if (p == 3) editJadwal();
        else if (p == 4) hapusJadwal();

    } while (p != 0);
}
