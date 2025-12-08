#include <stdio.h>
#define MAX 50

struct inventory
{
    char jenis[20];
    char namaBarang[50];
    int stok;

};
struct inventory barang[MAX];
int jumlah = 0;

//nambah barang
void tambahBarang(){
printf ("Isi Dengan Seksama Ya\n");
printf("Masukan Jenis Barang : ");
scanf("%s",barang[jumlah].jenis);
printf("Masukan Nama Barang : ");
scanf("%s",barang[jumlah].namaBarang);
printf("Masukan stok Barang : ");
scanf("%s",barang[jumlah].stok);

jumlah++;
printf("Berhasil Ditambahkan :)\n");
}

//lihat barang























int main(){
printf("===============================\n");
printf("Kelengkapan Posyandu\n");
printf("===============================\n");

}