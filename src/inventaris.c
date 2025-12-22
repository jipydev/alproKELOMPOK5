#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inventaris.h"
#include "utils.h"

#define BATAS 100
#define FILE_INV  "db/inventaris.txt"

typedef struct 
{
    int id;
    char namaBarang[50];
    int stok;
}inventaris;


//id
int generateId() {
    FILE *fp = fopen(FILE_INV, "r");
    if (!fp) return 1;

    inventaris i;
    int lastId = 0;

    while (fscanf(fp, "%d %s %d",
                  &i.id, i.namaBarang, &i.stok) == 3) {
        if (i.id > lastId)
            lastId = i.id;
    }

    fclose(fp);
    return lastId + 1;
}
//Fitur tambah 
void tambahInv() {
    clearScreen();
    FILE *fp = fopen(FILE_INV, "r");
    int count = 0;
    inventaris temp;

    if (fp) {
        while (fscanf(fp, "%d %s %d", &temp.id, temp.namaBarang, &temp.stok) == 3)
            count++;
        fclose(fp);
    }

    if (count >= BATAS) {
        printf("Inventory penuh!\n");
        return;
    }

    inventaris i;
    i.id = generateId();
printf("=====================================\n");
printf("|        TAMBAH DATA BARANG          |\n");
printf("=====================================\n");
printf("| Nama Barang : ");
scanf(" %s", i.namaBarang);
printf("| Jumlah      : ");
scanf("%d", &i.stok);
printf("=====================================\n");
 getchar();

    fp = fopen(FILE_INV, "a");
if (!fp) {
    printf("Gagal membuka file!\n");
    return;
}

fprintf(fp, "%d %s %d\n", i.id, i.namaBarang, i.stok);
fclose(fp);

printf("Data Berhasil Disimpan");
pauseScreen();
 
}
// Tampilkan semua data
void tampilkanSemuaInv() {
    clearScreen();
     FILE *fp = fopen(FILE_INV, "r");
    if (!fp) {
        printf("Belum ada data.\n");
        return;
    }

    inventaris i;

    printf("\n      === DATA INVENTARIS ===    \n");
printf("------------------------------------------------\n");
printf("| %-4s | %-20s | %-6s |\n", "ID", "Nama Barang", "Stok");
printf("------------------------------------------------\n");

while (fscanf(fp, "%d %s %d", &i.id, i.namaBarang, &i.stok) == 3) {
    printf("| %-4d | %-20s | %-6d |\n",
           i.id, i.namaBarang, i.stok);
}

printf("------------------------------------------------\n");
printf("Tekan 0 Untuk Melanjutkan..");
getchar();
    fclose(fp);
}
void editInv(){
clearScreen();

    FILE *fp, *temp;
    inventaris i;
    int idCari, found = 0;

    printf("=====================================\n");
    printf("|            EDIT BARANG             |\n");
    printf("=====================================\n");
    printf("Masukkan ID yang ingin diedit: ");
    scanf("%d", &idCari);
    printf("=====================================\n");

    fp = fopen(FILE_INV, "r");
    temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Gagal membuka file!\n");
        return;
    }

    while (fscanf(fp, "%d %s %d",
                  &i.id, i.namaBarang, &i.stok) == 3) {

        if (i.id == idCari) {
            found = 1;
            printf("Data ditemukan!\n");
            printf("Nama baru   : ");
            scanf("%s", i.namaBarang);
            printf("Jumlah baru : ");
            scanf("%d", &i.stok);
        }

        fprintf(temp, "%d %s %d\n",
                i.id, i.namaBarang, i.stok);
    }

    fclose(fp);
    fclose(temp);

    if (found) {
        remove(FILE_INV);
        rename("temp.txt", FILE_INV);
        printf("\nData berhasil diedit!\n");
    } else {
        remove("temp.txt");
        printf("\nID tidak ditemukan!\n");
    }

    printf("\nTekan 0 untuk kembali ke menu...");
    int back;
    do {
        scanf("%d", &back);
    } while (back != 0);
}
void hapusInv() {
    clearScreen();

    FILE *fp, *temp;
    inventaris i, barangHapus;
    int idHapus;
    int ditemukan = 0;

    /* ================== TAMPILKAN SEMUA DATA ================== */
    fp = fopen(FILE_INV, "r");
    if (!fp) {
        printf("Data inventaris masih kosong!\n");
        pauseScreen();
        return;
    }

    printf("================================================\n");
    printf("|              DATA INVENTARIS                 |\n");
    printf("================================================\n");
    printf("| %-4s | %-20s | %-6s |\n", "ID", "Nama Barang", "Stok");
    printf("------------------------------------------------\n");

    while (fscanf(fp, "%d %49s %d",
                  &i.id, i.namaBarang, &i.stok) == 3) {
        printf("| %-4d | %-20s | %-6d |\n",
               i.id, i.namaBarang, i.stok);
    }

    printf("------------------------------------------------\n");
    fclose(fp);
    pauseScreen();

    /* ================== INPUT ID HAPUS ================== */
    printf("\n=====================================\n");
    printf("|            HAPUS BARANG            |\n");
    printf("=====================================\n");
    printf("Masukkan ID yang ingin dihapus : ");
    scanf("%d", &idHapus);

    fp = fopen(FILE_INV, "r");
    temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Gagal membuka file.\n");
        pauseScreen();
        return;
    }

    /* ================== PROSES HAPUS ================== */
    while (fscanf(fp, "%d %49s %d",
                  &i.id, i.namaBarang, &i.stok) == 3) {

        if (i.id == idHapus) {
            barangHapus = i;
            ditemukan = 1;
        } else {
            fprintf(temp, "%d %s %d\n",
                    i.id, i.namaBarang, i.stok);
        }
    }

    fclose(fp);
    fclose(temp);

    /* ================== HASIL ================== */
    if (ditemukan) {
        remove(FILE_INV);
        rename("temp.txt", FILE_INV);

        printf("\nBarang BERHASIL dihapus:\n");
        printf("------------------------------------------------\n");
        printf("| %-4s | %-20s | %-6s |\n", "ID", "Nama Barang", "Stok");
        printf("------------------------------------------------\n");
        printf("| %-4d | %-20s | %-6d |\n",
               barangHapus.id,
               barangHapus.namaBarang,
               barangHapus.stok);
        printf("------------------------------------------------\n");
    } else {
        remove("temp.txt");
        printf("\n[!] ID tidak ditemukan.\n");
    }

    pauseScreen();
}
void cariInv() {


    clearScreen();
    FILE *fp, *temp;
    char namaInv[50];
    inventaris i;
    int ditemukan = 0;

printf("===============================================\n");
printf("|            CARI BARANG                       |\n");
printf("===============================================\n");
printf("Masukkan Nama barang yang ingin dicari: ");
scanf("%s", &namaInv);
printf("================================================\n");

    fp = fopen(FILE_INV, "r");
    if (!fp) {
        printf("Data belum ada!\n");
        return;
    }
    printf("------------------------------------------------\n");
    printf("| %-4s | %-20s | %-6s |\n", "ID", "Nama Barang", "Stok");
    printf("------------------------------------------------\n");

    while (fscanf(fp, "%d %s %d",
                  &i.id, i.namaBarang, &i.stok) == 3) {

        if (strcmp(i.namaBarang, namaInv) == 0) {
            printf("| %-4d | %-20s | %-6d |\n",
                   i.id, i.namaBarang, i.stok);
            ditemukan = 1;
            break;
        }
    }

    if (!ditemukan) {
        printf("| %-4s | %-20s | %-6s |\n", "-", "Data tidak ditemukan", "-");
    }

    printf("------------------------------------------------\n");
    fclose(fp);

    printf("\nTekan 0 untuk kembali ke menu...");
    getchar();
    getchar();
}





//Menu utama 
void menuInventaris(){
    
int pilih;
do{
    clearScreen();
printf("|=========================|\n");
printf("|       Inventory         |\n");
printf("|=========================|\n");
printf("|1.Lihat Barang           |\n");
printf("|-------------------------|\n");
printf("|2.Tambah Barang          |\n");
printf("|-------------------------|\n");
printf("|3.Edit Barang            |\n");
printf("|-------------------------|\n");
printf("|4.Hapus Barang           |\n");
printf("|-------------------------|\n");
printf("|5.Cari Inventory         |\n");
printf("|-------------------------|\n");
printf("|0.Keluar                 |\n");
printf("|-------------------------|\n");

pilih=inputInt("Pilihan Anda : ");
if (pilih==1) tampilkanSemuaInv();
else if (pilih==2) tambahInv();
else if (pilih==3) editInv();
else if (pilih==4) hapusInv();
else if (pilih==5) cariInv();

} while (pilih !=0);
}









