#ifndef DATA_WARGA_H
#define DATA_WARGA_H

#define MAX_WARGA 100

//STRUCT
typedef struct {
    int id;
    char nama[50];
    char alamat[100];
    int umur;
} Warga;

/* ===== CORE DATA ===== */
void loadDataWarga();
void saveDataWarga();

/* ===== CRUD ===== */
void tambahWarga();
void lihatWarga();
void editWarga();
void hapusWarga();

/* ===== SUB MENU ===== */
void tampilkanSemuaWarga();
void cariWargaById();
void cariWargaByNama();

/* ===== MENU ===== */
void menuWarga();

#endif
