#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cek_stunting.h"
#include "utils.h"

#define MAX_STUNTING 100
#define FILE_STUNTING "db/data_stunting.txt"

//ARRAY OF STRUCT//
Stunting dataStunting[MAX_STUNTING];
int jumlahStunting = 0;

//LOAD & SAVE (PERSISTENSI)//
void loadStunting() {
    FILE *f = fopen(FILE_STUNTING, "r");
    jumlahStunting = 0;

    if (!f) return;

    while (fscanf(f, "%d|%49[^|]|%d|%f|%19[^\n]\n",
           &dataStunting[jumlahStunting].id,
           dataStunting[jumlahStunting].nama,
           &dataStunting[jumlahStunting].umur,
           &dataStunting[jumlahStunting].tinggi,
           dataStunting[jumlahStunting].status) != EOF) {

        jumlahStunting++;
        if (jumlahStunting >= MAX_STUNTING) break;
    }
    fclose(f);
}

void saveStunting() {
    FILE *f = fopen(FILE_STUNTING, "w");
    if (!f) return;

    for (int i = 0; i < jumlahStunting; i++) {
        fprintf(f, "%d|%s|%d|%.2f|%s\n",
            dataStunting[i].id,
            dataStunting[i].nama,
            dataStunting[i].umur,
            dataStunting[i].tinggi,
            dataStunting[i].status);
    }
    fclose(f);
}

//LOGIKA WHO (TB/U)//
void tentukanStatus(Stunting *s) {
    float standar;

    if (s->umur <= 12) standar = 75;
    else if (s->umur <= 24) standar = 85;
    else if (s->umur <= 36) standar = 95;
    else if (s->umur <= 48) standar = 102;
    else standar = 110;

    if (s->tinggi < standar)
        strcpy(s->status, "STUNTING");
    else
        strcpy(s->status, "NORMAL");
}

//SEARCHING (SEQUENTIAL)//
int cariIndexStunting(int id) {
    for (int i = 0; i < jumlahStunting; i++) {
        if (dataStunting[i].id == id)
            return i;
    }
    return -1;
}
//SORTING (BUBBLE SORT)//
void sortStuntingByNama() {
    for (int i = 0; i < jumlahStunting - 1; i++) {
        for (int j = 0; j < jumlahStunting - i - 1; j++) {
            if (strcmp(dataStunting[j].nama,
                       dataStunting[j + 1].nama) > 0) {
                Stunting tmp = dataStunting[j];
                dataStunting[j] = dataStunting[j + 1];
                dataStunting[j + 1] = tmp;
            }
        }
    }
}

//TAMBAH DATA//
void tambahStunting() {
    clearScreen();
    loadStunting();

    printf("========================================\n");
    printf("        TAMBAH DATA STUNTING             \n");
    printf("========================================\n\n");

    int id = inputInt("Masukkan ID (0 = batal) : ");
    if (id == 0) {
        printf("\n========================================\n");
        printf("Proses dibatalkan.\n");
        printf("========================================\n");
        pauseScreen();
        return;
    }

    if (cariIndexStunting(id) != -1) {
        printf("\n========================================\n");
        printf("ID sudah terdaftar!\n");
        printf("========================================\n");
        pauseScreen();
        return;
    }

    Stunting s;
    s.id = id;

    printf("\n============== INPUT DATA ===============\n");

    inputString("Nama Anak            : ", s.nama, sizeof(s.nama));
    s.umur = inputInt("Umur (bulan, max 59) : ");

    if (s.umur < 0 || s.umur > 59) {
        printf("\n========================================\n");
        printf("Umur balita harus 0 - 59 bulan!\n");
        printf("========================================\n");
        pauseScreen();
        return;
    }

    s.tinggi = inputFloat("Tinggi Badan (cm)    : ");

    printf("========================================\n");

    tentukanStatus(&s);

    dataStunting[jumlahStunting++] = s;
    saveStunting();

    printf("\n========================================\n");
    printf("Data stunting berhasil ditambahkan!\n");
    printf("========================================\n");
    pauseScreen();
}

//LIHAT DATA//
void tampilkanSemuaStunting() {
    clearScreen();
    loadStunting();
    sortStuntingByNama();

    printf("======================================================================\n");
    printf("        DATA CEK STUNTING BALITA         \n");
    printf("======================================================================\n\n");

    if (jumlahStunting == 0) {
        printf("Belum ada data stunting.\n");
        printf("=================================================================\n");
        pauseScreen();
        return;
    }

    printf("======================================================================\n");
    printf("| %-4s | %-20s | %-10s | %-10s | %-10s |\n",
           "ID", "Nama Anak", "Umur", "Tinggi", "Status");
    printf("======================================================================\n");

    for (int i = 0; i < jumlahStunting; i++) {
        printf("| %-4d | %-20s | %-7d bln | %-7.1f cm | %-10s |\n",
               dataStunting[i].id,
               dataStunting[i].nama,
               dataStunting[i].umur,
               dataStunting[i].tinggi,
               dataStunting[i].status);
    }

    printf("======================================================================\n");
    printf("Total Data : %d Balita\n", jumlahStunting);
    printf("======================================================================\n");

    pauseScreen();
}

void cariStuntingByNama() {
    clearScreen();
    loadStunting();

    char key[50];
    inputString("Masukkan nama: ", key, sizeof(key));

    if (strlen(key) == 0) {
        printf("\n[!] Nama tidak boleh kosong!\n");
        pauseScreen();
        return;
    }

    int found = 0;

    printf("\n===============================================================\n");
    printf("| %-4s | %-20s | %-8s | %-15s |\n",
           "ID", "Nama", "TB(cm)", "Status");
    printf("===============================================================\n");

    for (int i = 0; i < jumlahStunting; i++) {
        if (strstr(dataStunting[i].nama, key)) {
            printf("| %-4d | %-20s | %-8.1f | %-15s |\n",
                   dataStunting[i].id,
                   dataStunting[i].nama,
                   dataStunting[i].tinggi,
                   dataStunting[i].status);
            found = 1;
        }
    }

    printf("===============================================================\n");

    if (!found) {
        printf("\n[!] Data dengan nama \"%s\" tidak ditemukan.\n", key);
    }

    pauseScreen();
}
void cariStuntingById() {
    clearScreen();
    loadStunting();

    printf("========================================\n");
    printf("        CARI STUNTING BERDASARKAN ID     \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID : ");
    int idx = cariIndexStunting(id);

    if (idx == -1) {
        printf("\n[!] Data tidak ditemukan!\n");
    } else {
        printf("\nData ditemukan:\n");
        printf("----------------------------------------\n");
        printf("ID     : %d\n", dataStunting[idx].id);
        printf("Nama   : %s\n", dataStunting[idx].nama);
        printf("Umur   : %d bulan\n", dataStunting[idx].umur);
        printf("Tinggi : %.1f cm\n", dataStunting[idx].tinggi);
        printf("Status : %s\n", dataStunting[idx].status);
        printf("----------------------------------------\n");
    }

    pauseScreen();
}


//EDIT & HAPUS//
void editStunting() {
    clearScreen();
    loadStunting();

    printf("============================================\n");
    printf("            EDIT DATA STUNTING               \n");
    printf("============================================\n");

    int id = inputInt("Masukkan ID yang akan diedit : ");
    int idx = cariIndexStunting(id);

    if (idx == -1) {
        printf("\n[!] ID tidak ditemukan!\n");
        pauseScreen();
        return;
    }

    printf("\n==============Data Lama===================\n");
    printf("--------------------------------------------\n");
    printf("Nama   : %s\n", dataStunting[idx].nama);
    printf("Umur   : %d bulan\n", dataStunting[idx].umur);
    printf("Tinggi : %.1f cm\n", dataStunting[idx].tinggi);
    printf("Status : %s\n", dataStunting[idx].status);
    printf("--------------------------------------------\n");

    printf("\nMasukkan Data Baru\n");
    inputString("Nama   : ", dataStunting[idx].nama, sizeof(dataStunting[idx].nama));
    dataStunting[idx].umur   = inputInt("Umur   (bulan): ");
    dataStunting[idx].tinggi = inputFloat("Tinggi (cm)  : ");
    tentukanStatus(&dataStunting[idx]);

    saveStunting();

    printf("\n[✓] Data berhasil diperbarui!\n");
    pauseScreen();
}

void hapusStunting() {
    clearScreen();
    loadStunting();

    printf("========================================\n");
    printf("          HAPUS DATA STUNTING            \n");
    printf("========================================\n");

    int id = inputInt("Masukkan ID yang akan dihapus : ");
    int idx = cariIndexStunting(id);

    if (idx == -1) {
        printf("\n[!] ID tidak ditemukan!\n");
        pauseScreen();
        return;
    }

    printf("\n=========Data yang dihapus:===========\n");
    printf("----------------------------------------\n");
    printf("ID     : %d\n", dataStunting[idx].id);
    printf("Nama   : %s\n", dataStunting[idx].nama);
    printf("Umur   : %d bulan\n", dataStunting[idx].umur);
    printf("Tinggi : %.1f cm\n", dataStunting[idx].tinggi);
    printf("Status : %s\n", dataStunting[idx].status);
    printf("----------------------------------------\n");

    for (int i = idx; i < jumlahStunting - 1; i++)
        dataStunting[i] = dataStunting[i + 1];

    jumlahStunting--;
    saveStunting();

    printf("\nData di atas berhasil dihapus!\n");
    pauseScreen();
}

//SUB MENU LIHAT//
void lihatStunting() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|       Data Stunting     |\n");
printf("|=========================|\n");
printf("|1.Lihat Semua            |\n");
printf("|-------------------------|\n");
printf("|2.Cari Stunting By Id    |\n");
printf("|-------------------------|\n");
printf("|3.Cari Stunting By Nama  |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tampilkanSemuaStunting();
        else if (p == 2) cariStuntingById();
        else if (p == 3) cariStuntingByNama();

    } while (p != 0);
}


//MENU UTAMA//
void menuStunting() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|       Stunting          |\n");
printf("|=========================|\n");
printf("|1.Tambah Data Stunting   |\n");
printf("|-------------------------|\n");
printf("|2.Lihat Data Stunting    |\n");
printf("|-------------------------|\n");
printf("|3.Edit Data Stunting     |\n");
printf("|-------------------------|\n");
printf("|4.Hapus Data Stunting    |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");;

        p = inputInt("Pilih: ");

        if (p == 1) tambahStunting();
        else if (p == 2) lihatStunting();
        else if (p == 3) editStunting();
        else if (p == 4) hapusStunting();

    } while (p != 0);
}