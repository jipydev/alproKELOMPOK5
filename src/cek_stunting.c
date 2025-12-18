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

    int id = inputInt("Masukkan ID (0=batal): ");
    if (id == 0) return;

    if (cariIndexStunting(id) != -1) {
        printf("ID sudah terdaftar!\n");
        pauseScreen();
        return;
    }

    Stunting s;
    s.id = id;
    inputString("Nama Anak  : ", s.nama, sizeof(s.nama));
    s.umur = inputInt("Umur (bulan, max 59): ");

    if (s.umur < 0 || s.umur > 59) {
        printf("Umur balita harus 0–59 bulan!\n");
        pauseScreen();
        return;
    }

    s.tinggi = inputFloat("Tinggi Badan (cm): ");
    tentukanStatus(&s);

    dataStunting[jumlahStunting++] = s;
    saveStunting();

    printf("Data stunting berhasil ditambahkan!\n");
    pauseScreen();
}

//LIHAT DATA//
void tampilkanSemuaStunting() {
    clearScreen();
    loadStunting();
    sortStuntingByNama();

    printf("=== DATA CEK STUNTING BALITA ===\n");
    for (int i = 0; i < jumlahStunting; i++) {
        printf("ID:%d | %s | Umur:%d bln | %.1f cm | %s\n",
            dataStunting[i].id,
            dataStunting[i].nama,
            dataStunting[i].umur,
            dataStunting[i].tinggi,
            dataStunting[i].status);
    }
    pauseScreen();
}

void cariStuntingById() {
    clearScreen();
    loadStunting();

    int id = inputInt("Masukkan ID: ");
    int idx = cariIndexStunting(id);

    if (idx == -1)
        printf("Data tidak ditemukan.\n");
    else
        printf("Nama:%s\nUmur:%d bln\nTinggi:%.1f cm\nStatus:%s\n",
            dataStunting[idx].nama,
            dataStunting[idx].umur,
            dataStunting[idx].tinggi,
            dataStunting[idx].status);

    pauseScreen();
}

void cariStuntingByNama() {
    clearScreen();
    loadStunting();

    char key[50];
    inputString("Masukkan nama: ", key, sizeof(key));

    int found = 0;
    for (int i = 0; i < jumlahStunting; i++) {
        if (strstr(dataStunting[i].nama, key)) {
            printf("ID:%d | %s | %s\n",
                dataStunting[i].id,
                dataStunting[i].nama,
                dataStunting[i].status);
            found = 1;
        }
    }

    if (!found) printf("Data tidak ditemukan.\n");
    pauseScreen();
}

//EDIT & HAPUS//
void editStunting() {
    clearScreen();
    loadStunting();

    int id = inputInt("Masukkan ID: ");
    int idx = cariIndexStunting(id);

    if (idx == -1) {
        printf("ID tidak ditemukan!\n");
        pauseScreen();
        return;
    }

    inputString("Nama baru  : ", dataStunting[idx].nama, sizeof(dataStunting[idx].nama));
    dataStunting[idx].umur = inputInt("Umur baru (bulan): ");
    dataStunting[idx].tinggi = inputFloat("Tinggi baru (cm): ");
    tentukanStatus(&dataStunting[idx]);

    saveStunting();
    printf("Data berhasil diupdate!\n");
    pauseScreen();
}

void hapusStunting() {
    clearScreen();
    loadStunting();

    int id = inputInt("Masukkan ID: ");
    int idx = cariIndexStunting(id);

    if (idx == -1) {
        printf("ID tidak ditemukan!\n");
        pauseScreen();
        return;
    }

    for (int i = idx; i < jumlahStunting - 1; i++)
        dataStunting[i] = dataStunting[i + 1];

    jumlahStunting--;
    saveStunting();

    printf("Data berhasil dihapus!\n");
    pauseScreen();
}

//SUB MENU LIHAT//
void lihatStunting() {
    int p;
    do {
        clearScreen();
        printf("=== LIHAT DATA STUNTING ===\n");
        printf("1. Tampilkan Semua\n");
        printf("2. Cari Berdasarkan ID\n");
        printf("3. Cari Berdasarkan Nama\n");
        printf("0. Kembali\n");

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
        printf("=== MENU CEK STUNTING BALITA ===\n");
        printf("1. Tambah Data\n");
        printf("2. Lihat Data\n");
        printf("3. Edit Data\n");
        printf("4. Hapus Data\n");
        printf("0. Kembali\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahStunting();
        else if (p == 2) lihatStunting();
        else if (p == 3) editStunting();
        else if (p == 4) hapusStunting();

    } while (p != 0);
}