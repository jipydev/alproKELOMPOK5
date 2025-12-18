#ifndef REKAM_MEDIS_H
#define REKAM_MEDIS_H

/* =======================
   STRUKTUR DATA KOMPLEKS
   ======================= */
typedef struct {
    int id;
    char nama[50];
    char keluhan[100];
} Rekam;

/* =======================
   MENU UTAMA
   ======================= */
void menuRekam();

/* =======================
   CRUD DATA
   ======================= */
void tambahRekam();
void lihatRekam();
void editRekam();
void hapusRekam();

/* =======================
   SUB MENU LIHAT
   ======================= */
void tampilkanSemuaRekam();
void cariRekamById();
void cariRekamByNama();

/* =======================
   UTILITAS INTERNAL
   ======================= */
void loadRekam();
void saveRekam();
int  cariIndexById(int id);
void sortRekamByNama();

#endif