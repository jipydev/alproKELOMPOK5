 #ifndef CEK_STUNTING_H
#define CEK_STUNTING_H

/* =======================
   STRUKTUR DATA KOMPLEKS
   ======================= */
typedef struct {
    int id;
    char nama[50];
    int umur;        // bulan (0–59)
    float tinggi;    // cm
    char status[20];
} Stunting;

/* =======================
   MENU UTAMA
   ======================= */
void menuStunting();

/* =======================
   CRUD
   ======================= */
void tambahStunting();
void lihatStunting();
void editStunting();
void hapusStunting();

/* =======================
   SUB MENU
   ======================= */
void tampilkanSemuaStunting();
void cariStuntingById();
void cariStuntingByNama();

/* =======================
   UTILITAS INTERNAL
   ======================= */
void loadStunting();
void saveStunting();
void tentukanStatus(Stunting *s);
int  cariIndexStunting(int id);
void sortStuntingByNama();

#endif