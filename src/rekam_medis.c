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

void saveRekam() {
    FILE *f = fopen(FILE_REKAM, "w");
    if (!f) return;

    for (int i = 0; i < jumlahRekam; i++) {
        fprintf(f, "%d|%s|%s\n",
            dataRekam[i].id,
            dataRekam[i].nama,
            dataRekam[i].keluhan);
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

    printf("========================================\n");
    printf("      TAMBAH DATA REKAM MEDIS            \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID Rekam (0 = batal) : ");
    if (id == 0) {
        printf("\nProses penambahan dibatalkan.\n");
        pauseScreen();
        return;
    }

    if (cariIndexById(id) != -1) {
        printf("\n[!] ID sudah digunakan!\n");
        pauseScreen();
        return;
    }

    Rekam r;
    r.id = id;

    printf("\n=========== INPUT DATA ===========\n");
    inputString("Nama Pasien : ", r.nama, sizeof(r.nama));
    inputString("Keluhan     : ", r.keluhan, sizeof(r.keluhan));
    printf("=================================\n");

    dataRekam[jumlahRekam++] = r;
    saveRekam();

    printf("\n[✓] Data rekam medis berhasil ditambahkan!\n");
    pauseScreen();
}

/* =======================
   LIHAT DATA
   ======================= */
void tampilkanSemuaRekam() {
    clearScreen();
    loadRekam();
    sortRekamByNama();

    printf("========================================\n");
    printf("           DATA REKAM MEDIS              \n");
    printf("========================================\n");

    if (jumlahRekam == 0) {
        printf("\nBelum ada data rekam medis.\n");
        printf("========================================\n");
        pauseScreen();
        return;
    }

    printf("| %-4s | %-20s | %-30s |\n",
           "ID", "Nama Pasien", "Keluhan");
    printf("========================================\n");

    for (int i = 0; i < jumlahRekam; i++) {
        printf("| %-4d | %-20s | %-30s |\n",
               dataRekam[i].id,
               dataRekam[i].nama,
               dataRekam[i].keluhan);
    }

    printf("========================================\n");
    printf("Total Rekam Medis : %d\n", jumlahRekam);
    printf("========================================\n");

    pauseScreen();
}

void cariRekamById() {
    clearScreen();
    loadRekam();

    printf("========================================\n");
    printf("     CARI DATA REKAM MEDIS BERDASARKAN ID\n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID Rekam : ");
    int idx = cariIndexById(id);

    if (idx == -1) {
        printf("\n[!] Data rekam medis dengan ID %d tidak ditemukan.\n", id);
    } else {
        printf("\n========== DATA REKAM MEDIS ==========\n");
        printf("ID       : %d\n", dataRekam[idx].id);
        printf("Nama     : %s\n", dataRekam[idx].nama);
        printf("Keluhan  : %s\n", dataRekam[idx].keluhan);
        printf("=====================================\n");
    }

    pauseScreen();
} 
void cariRekamByNama() {
    clearScreen();
    loadRekam();

    printf("========================================\n");
    printf("     CARI DATA REKAM MEDIS BERDASARKAN NAMA\n");
    printf("========================================\n");

    char key[50];
    inputString("Masukkan nama pasien : ", key, sizeof(key));

    int found = 0;

    printf("\n| %-4s | %-20s | %-25s |\n",
           "ID", "Nama Pasien", "Keluhan");
    printf("========================================\n");

    for (int i = 0; i < jumlahRekam; i++) {
        if (strstr(dataRekam[i].nama, key)) {
            printf("| %-4d | %-20s | %-25s |\n",
                   dataRekam[i].id,
                   dataRekam[i].nama,
                   dataRekam[i].keluhan);
            found = 1;
        }
    }

    if (!found) {
        printf("\n[!] Data rekam medis dengan nama \"%s\" tidak ditemukan.\n", key);
    } else {
        printf("========================================\n");
    }

    pauseScreen();
}

void lihatRekam() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|     Data Rekam Medis    |\n");
printf("|=========================|\n");
printf("|1.Lihat Semua            |\n");
printf("|-------------------------|\n");
printf("|2.Cari Data By Id        |\n");
printf("|-------------------------|\n");
printf("|3.Cari Data By Nama      |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");
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

    printf("========================================\n");
    printf("          EDIT REKAM MEDIS               \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID Rekam : ");
    int idx = cariIndexById(id);

    if (idx == -1) {
        printf("\n[!] ID tidak ditemukan!\n");
        pauseScreen();
        return;
    }

    printf("\nData Lama\n");
    printf("----------------------------------------\n");
    printf("ID      : %d\n", dataRekam[idx].id);
    printf("Nama    : %s\n", dataRekam[idx].nama);
    printf("Keluhan : %s\n", dataRekam[idx].keluhan);
    printf("----------------------------------------\n");

    printf("\nMasukkan Data Baru\n");
    inputString("Nama Baru    : ", dataRekam[idx].nama, sizeof(dataRekam[idx].nama));
    inputString("Keluhan Baru : ", dataRekam[idx].keluhan, sizeof(dataRekam[idx].keluhan));

    saveRekam();

    printf("\n[✓] Data rekam medis berhasil diperbarui!\n");
    pauseScreen();
}

/* =======================
   HAPUS DATA
   ======================= */
void hapusRekam() {
    clearScreen();
    loadRekam();

    printf("========================================\n");
    printf("          HAPUS DATA REKAM MEDIS         \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID Rekam : ");
    int idx = cariIndexById(id);

    if (idx == -1) {
        printf("\n[!] ID tidak ditemukan!\n");
        pauseScreen();
        return;
    }

    /* Tampilkan data yang akan dihapus */
    printf("\nData yang dihapus:\n");
    printf("----------------------------------------\n");
    printf("ID      : %d\n", dataRekam[idx].id);
    printf("Nama    : %s\n", dataRekam[idx].nama);
    printf("Keluhan : %s\n", dataRekam[idx].keluhan);
    printf("----------------------------------------\n");

    for (int i = idx; i < jumlahRekam - 1; i++) {
        dataRekam[i] = dataRekam[i + 1];
    }
    jumlahRekam--;

    saveRekam();

    printf("\n[✓] Data rekam medis berhasil dihapus!\n");
    pauseScreen();
}

/* =======================
   MENU UTAMA
   ======================= */
void menuRekam() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|       Rekam Medis       |\n");
printf("|=========================|\n");
printf("|1.Tambah Data            |\n");
printf("|-------------------------|\n");
printf("|2.Lihat Data             |\n");
printf("|-------------------------|\n");
printf("|3.Edit Data              |\n");
printf("|-------------------------|\n");
printf("|4.Hapus Data             |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahRekam();
        else if (p == 2) lihatRekam();
        else if (p == 3) editRekam();
        else if (p == 4) hapusRekam();

    } while (p != 0);
}