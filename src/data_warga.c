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
printf("===========================================\n");
printf("|            HAPUS BARANG                  |\n");
printf("===========================================\n");
int id = inputInt("Masukan Id (0 untuk kembali) : ");
printf("===========================================\n");
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

printf("===============================================================\n");
printf("| %-4s | %-20s | %-25s | %-4s |\n",
       "ID", "Nama", "Alamat", "Umur");
printf("===============================================================\n");

for (int i = 0; i < jumlahWarga; i++) {
    printf("| %-4d | %-20s | %-25s | %-4d |\n",
           warga[i].id,
           warga[i].nama,
           warga[i].alamat,
           warga[i].umur);
}

printf("===============================================================\n");
    pauseScreen();
}

/* ===== SEARCH ===== */
void cariWargaById() {
    clearScreen();
    int id = inputInt("Masukkan ID: ");

    for (int i = 0; i < jumlahWarga; i++) {
        if (warga[i].id == id) {

            printf("=====================================\n");
            printf("|        DATA WARGA DITEMUKAN        |\n");
            printf("=====================================\n");
            printf("| ID     : %-25d |\n", warga[i].id);
            printf("| Nama   : %-25s |\n", warga[i].nama);
            printf("| Alamat : %-25s |\n", warga[i].alamat);
            printf("| Umur   : %-25d |\n", warga[i].umur);
            printf("=====================================\n");

            pauseScreen();
            return;
        }
    }

    printf("=====================================\n");
    printf("|       DATA TIDAK DITEMUKAN         |\n");
    printf("=====================================\n");
    pauseScreen();
}

void cariWargaByNama() {
    clearScreen();

    char key[50];
    inputString("Masukkan Nama: ", key, sizeof(key));
    key[strcspn(key, "\n")] = 0;

    int found = 0;

    printf("===============================================================\n");
    printf("|                    HASIL PENCARIAN WARGA                     |\n");
    printf("===============================================================\n");
    printf("| %-4s | %-20s | %-25s | %-4s |\n",
           "ID", "Nama", "Alamat", "Umur");
    printf("===============================================================\n");

    for (int i = 0; i < jumlahWarga; i++) {
        if (strstr(warga[i].nama, key)) {
            printf("| %-4d | %-20s | %-25s | %-4d |\n",
                   warga[i].id,
                   warga[i].nama,
                   warga[i].alamat,
                   warga[i].umur);
            found = 1;
        }
    }

    printf("===============================================================\n");

    if (!found) {
        printf("|               DATA TIDAK DITEMUKAN                           |\n");
        printf("===============================================================\n");
    }

    pauseScreen();
}

/* ===== EDIT ===== */
void editWarga() {
    clearScreen();
    int id = inputInt("Masukkan ID Warga: ");

    for (int i = 0; i < jumlahWarga; i++) {
        if (warga[i].id == id) {

         printf("=====================================\n");
         printf("|        DATA WARGA DITEMUKAN        |\n");
         printf("=====================================\n");
         printf("| ID     : %-25d |\n", warga[i].id);
         printf("| Nama   : %-25s |\n", warga[i].nama);
         printf("| Alamat : %-25s |\n", warga[i].alamat);
         printf("| Umur   : %-25d |\n", warga[i].umur);
         printf("=====================================\n\n");

         printf("========= MASUKKAN DATA BARU =========\n");
            inputString("Nama baru   : ", warga[i].nama, sizeof(warga[i].nama));
            inputString("Alamat baru : ", warga[i].alamat, sizeof(warga[i].alamat));
            warga[i].umur = inputInt("Umur baru   : ");

 warga[i].nama[strcspn(warga[i].nama, "\n")] = 0;
warga[i].alamat[strcspn(warga[i].alamat, "\n")] = 0;

 saveDataWarga();

            printf("\n=====================================\n");
            printf("|        DATA BERHASIL DIPERBARUI    |\n");
            printf("=====================================\n");

            pauseScreen();
            return;
        }
    }

    printf("=====================================\n");
    printf("|        ID TIDAK DITEMUKAN          |\n");
    printf("=====================================\n");
    pauseScreen();
}

/* ===== HAPUS ===== */
void hapusWarga() {
    clearScreen();
    int id = inputInt("Masukkan ID Warga: ");

    for (int i = 0; i < jumlahWarga; i++) {
        if (warga[i].id == id) {

            printf("=====================================\n");
            printf("|        DATA WARGA DITEMUKAN        |\n");
            printf("=====================================\n");
            printf("| ID     : %-25d |\n", warga[i].id);
            printf("| Nama   : %-25s |\n", warga[i].nama);
            printf("| Alamat : %-25s |\n", warga[i].alamat);
            printf("| Umur   : %-25d |\n", warga[i].umur);
            printf("=====================================\n");

           
                for (int j = i; j < jumlahWarga - 1; j++) {
                    warga[j] = warga[j + 1];
                
                jumlahWarga--;
                saveDataWarga();

                printf("\n=====================================\n");
                printf("|        DATA BERHASIL DIHAPUS       |\n");
                printf("=====================================\n");
            }

            pauseScreen();
            return;
        }
    }
    


    printf("=====================================\n");
    printf("|        ID TIDAK DITEMUKAN          |\n");
    printf("=====================================\n");
    pauseScreen();
}

/* ===== MENU ===== */
void lihatWarga() {
    int p;
    do {
        clearScreen();
printf("|=========================|\n");
printf("|       Data Warga        |\n");
printf("|=========================|\n");
printf("|1.Lihat Semua            |\n");
printf("|-------------------------|\n");
printf("|2.Cari By Id             |\n");
printf("|-------------------------|\n");
printf("|3.Cari By Nama           |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");
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
printf("|=========================|\n");
printf("|       Menu Warga        |\n");
printf("|=========================|\n");
printf("|1.Tambah Warga           |\n");
printf("|-------------------------|\n");
printf("|2.Lihat Data Warga       |\n");
printf("|-------------------------|\n");
printf("|3.Edit Data Warga        |\n");
printf("|-------------------------|\n");
printf("|4.Hapus Data Warga       |\n");
printf("|-------------------------|\n");
printf("|0.Exit                   |\n");
printf("|-------------------------|\n");

        p = inputInt("Pilih: ");

        if (p == 1) tambahWarga();
        else if (p == 2) lihatWarga();
        else if (p == 3) editWarga();
        else if (p == 4) hapusWarga();
    } while (p != 0);
}
