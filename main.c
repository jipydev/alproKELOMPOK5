#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WHO_CSV_PATH "data/who_hfa.csv"
#define CHILD_CSV "children.csv"
#define MEAS_CSV "measurements.csv"

typedef struct {
    int age_months;
    char sex; /* 'M' or 'F' */
    double median;
    double sd;
} WhoRow;

typedef struct {
    WhoRow *rows;
    size_t n;
} WhoTable;

/* ---------- Utilities ---------- */

static char *trim(char *s) {
    if (!s) return s;
    /* trim left */
    while (*s && (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n')) s++;
    /* trim right */
    char *end = s + strlen(s) - 1;
    while (end >= s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
        *end = '\0';
        end--;
    }
    return s;
}

/* parse CSV line token safely */
static char *next_token(char **sp, char delim) {
    if (!sp || !*sp) return NULL;
    char *s = *sp;
    if (!*s) return NULL;
    char *p = s;
    while (*p && *p != delim && *p != '\r' && *p != '\n') p++;
    size_t len = p - s;
    char *tok = malloc(len + 1);
    if (!tok) return NULL;
    strncpy(tok, s, len);
    tok[len] = '\0';
    if (*p == delim) p++;
    *sp = (*p) ? p : p;
    return trim(tok);
}

/* read full line (handles long lines) */
static char *read_line(FILE *f) {
    size_t cap = 256;
    size_t len = 0;
    char *buf = malloc(cap);
    if (!buf) return NULL;
    while (fgets(buf + len, (int)(cap - len), f)) {
        len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n') break;
        /* need bigger buffer */
        cap *= 2;
        char *nb = realloc(buf, cap);
        if (!nb) { free(buf); return NULL; }
        buf = nb;
    }
    if (len == 0 && feof(f)) { free(buf); return NULL; }
    return buf;
}

/* ---------- Load WHO ref ---------- */

static void who_init(WhoTable *t) {
    t->rows = NULL;
    t->n = 0;
}

static void who_free(WhoTable *t) {
    free(t->rows);
    t->rows = NULL;
    t->n = 0;
}

static int who_load(WhoTable *t, const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    /* read header first line (skip) */
    char *line = read_line(f);
    if (line) { free(line); line = NULL; }
    size_t cap = 0;
    while ((line = read_line(f)) != NULL) {
        char *p = line;
        char *tok;
        tok = next_token(&p, ','); if (!tok) { free(line); continue; }
        int age = atoi(tok); free(tok);
        tok = next_token(&p, ','); if (!tok) { free(line); continue; }
        char sex = (tok[0] == 'F' || tok[0] == 'f') ? 'F' : 'M'; free(tok);
        tok = next_token(&p, ','); if (!tok) { free(line); continue; }
        double median = atof(tok); free(tok);
        tok = next_token(&p, ','); /* maybe none left */ 
        double sd = 0.0;
        if (tok) { sd = atof(tok); free(tok); }
        /* store */
        WhoRow r = { age, sex, median, sd };
        WhoRow *nr = realloc(t->rows, (t->n + 1) * sizeof(WhoRow));
        if (!nr) { free(line); fclose(f); return 0; }
        t->rows = nr;
        t->rows[t->n] = r;
        t->n++;
        free(line);
    }
    fclose(f);
    return 1;
}

/* find a median/sd for age/sex; tries exact then +/- upto 6 months */
static int who_lookup(WhoTable *t, int age_months, char sex, double *median_out, double *sd_out) {
    if (!t || t->n == 0) return 0;
    /* direct search helper */
    for (int d = 0; d <= 6; ++d) {
        /* order: 0, +1, -1, +2, -2, ... */
        int offsets[2] = { d, -d };
        for (int k = 0; k < 2; ++k) {
            int off = offsets[k];
            int target = age_months + off;
            for (size_t i = 0; i < t->n; ++i) {
                if (t->rows[i].age_months == target && t->rows[i].sex == sex) {
                    *median_out = t->rows[i].median;
                    *sd_out = t->rows[i].sd;
                    return 1;
                }
            }
            if (d == 0) break; /* avoid duplicate search */
        }
    }
    return 0;
}

/* ---------- CSV helpers for app data ---------- */

/* get next available id by scanning file's last line */
static long next_id_in_csv(const char *path) {
    FILE *f = fopen(path, "r");
    if (!f) return 1;
    char *line = NULL;
    long last_id = 0;
    while ((line = read_line(f)) != NULL) {
        char *p = line;
        char *tok = next_token(&p, ',');
        if (tok) {
            long id = atol(tok);
            if (id > last_id) last_id = id;
            free(tok);
        }
        free(line);
    }
    fclose(f);
    return last_id + 1;
}

/* ---------- App logic / UI ---------- */

static void ensure_data_dir_exists() {
    /* naive: try to open WHO_CSV dir; if not exists, ignore.
       we will create CSVs in current folder; user can create data/ for WHO CSV */
}

/* Add child -> append to children.csv
   columns: id,name,nik,sex,birth_date,notes
*/
static void add_child() {
    char name[256], nik[64], sex_s[8], birth[32], notes[256];
    printf("Tambah Anak\n");
    printf("Nama: "); fgets(name, sizeof(name), stdin); trim(name);
    printf("NIK (boleh kosong): "); fgets(nik, sizeof(nik), stdin); trim(nik);
    printf("Jenis Kelamin (M/F): "); fgets(sex_s, sizeof(sex_s), stdin); trim(sex_s);
    printf("Tanggal Lahir (YYYY-MM-DD) (boleh kosong): "); fgets(birth, sizeof(birth), stdin); trim(birth);
    printf("Catatan (boleh kosong): "); fgets(notes, sizeof(notes), stdin); trim(notes);
    char sex = (sex_s[0] == 'F' || sex_s[0] == 'f') ? 'F' : 'M';
    long id = next_id_in_csv(CHILD_CSV);
    FILE *f = fopen(CHILD_CSV, "a");
    if (!f) {
        perror("Gagal membuka children.csv untuk ditulis");
        return;
    }
    fprintf(f, "%ld,%s,%s,%c,%s,%s\n", id, name, nik, sex, birth, notes);
    fclose(f);
    printf("Anak ditambahkan dengan ID %ld\n", id);
}

/* List children */
static void list_children() {
    FILE *f = fopen(CHILD_CSV, "r");
    if (!f) {
        printf("Belum ada data anak (file children.csv tidak ditemukan).\n");
        return;
    }
    printf("Daftar Anak:\n");
    char *line;
    while ((line = read_line(f)) != NULL) {
        char *p = line;
        char *tok;
        tok = next_token(&p, ','); if (!tok) { free(line); continue; } long id = atol(tok); free(tok);
        tok = next_token(&p, ','); char *name = tok ? tok : strdup(""); 
        tok = next_token(&p, ','); char *nik = tok ? tok : strdup("");
        tok = next_token(&p, ','); char sex = tok ? tok[0] : 'M'; if (tok) free(tok);
        tok = next_token(&p, ','); char *birth = tok ? tok : strdup("");
        tok = next_token(&p, ','); char *notes = tok ? tok : strdup("");
        printf("ID %ld - %s (Sex %c) - NIK:%s - DOB:%s\n", id, name, sex, nik, birth);
        free(line);
        if (name) free(name);
        if (nik) free(nik);
        if (birth) free(birth);
        if (notes) free(notes);
    }
    fclose(f);
}

/* Add measurement
   measurement columns: id,child_id,date,age_months,height_cm,weight_kg,zscore,status
*/
static void add_measurement(WhoTable *who) {
    char child_id_s[32], date[32], age_s[16], height_s[32], weight_s[32];
    printf("Tambah Pengukuran\n");
    printf("Masukkan ID Anak: "); fgets(child_id_s, sizeof(child_id_s), stdin); trim(child_id_s);
    long child_id = atol(child_id_s);
    if (child_id <= 0) { printf("ID tidak valid\n"); return; }
    printf("Tanggal (YYYY-MM-DD) (boleh kosong): "); fgets(date, sizeof(date), stdin); trim(date);
    printf("Usia (bulan): "); fgets(age_s, sizeof(age_s), stdin); trim(age_s);
    printf("Tinggi/Panjang (cm): "); fgets(height_s, sizeof(height_s), stdin); trim(height_s);
    printf("Berat (kg) (boleh kosong): "); fgets(weight_s, sizeof(weight_s), stdin); trim(weight_s);
    int age_months = atoi(age_s);
    double height = atof(height_s);
    double weight = (strlen(weight_s)>0) ? atof(weight_s) : NAN;

    /* need sex of child: find in children.csv */
    FILE *fc = fopen(CHILD_CSV, "r");
    char sex = 'M';
    int found = 0;
    if (fc) {
        char *line;
        while ((line = read_line(fc)) != NULL) {
            char *p = line;
            char *tok = next_token(&p, ','); long id = atol(tok); free(tok);
            if (id == child_id) {
                tok = next_token(&p, ','); char *name = tok; /* name */
                tok = next_token(&p, ','); char *nik = tok;
                tok = next_token(&p, ','); if (tok) { sex = tok[0]; free(tok); } else sex = 'M';
                found = 1;
                free(line);
                break;
            }
            free(line);
        }
        fclose(fc);
    }
    if (!found) {
        printf("Anak dengan ID %ld tidak ditemukan di %s\n", child_id, CHILD_CSV);
        return;
    }

    /* compute zscore if possible */
    double z = NAN;
    char status[64] = "Unknown";
    double median, sd;
    if (who && who->n > 0 && who_lookup(who, age_months, sex, &median, &sd) && sd > 0.0) {
        z = (height - median) / sd;
        /* classify */
        if (!isnan(z)) {
            if (z <= -3.0) strcpy(status, "Severely Stunted");
            else if (z < -2.0) strcpy(status, "Stunted");
            else strcpy(status, "Normal");
        }
    } else {
        strcpy(status, "Unknown");
    }

    long id = next_id_in_csv(MEAS_CSV);
    FILE *fm = fopen(MEAS_CSV, "a");
    if (!fm) {
        perror("Gagal membuka measurements.csv untuk ditulis");
        return;
    }
    /* write: id,child_id,date,age_months,height_cm,weight_kg,zscore,status */
    if (isnan(z))
        fprintf(fm, "%ld,%ld,%s,%d,%.2f,%s,,%s\n", id, child_id, date, age_months, height,
                (isnan(weight) ? "" : ""), status);
    else
        fprintf(fm, "%ld,%ld,%s,%d,%.2f,%s,%.4f,%s\n", id, child_id, date, age_months, height,
                (isnan(weight) ? "" : ""), z, status);
    fclose(fm);
    printf("Pengukuran disimpan (ID %ld). Z-score: %s, Status: %s\n", id,
           isnan(z) ? "N/A" : (char[32]){0}, status);
    if (!isnan(z)) printf("  (Z = %.4f, median=%.2f, sd=%.2f)\n", z, median, sd);
}

/* view measurements of a child */
static void view_measurements() {
    char child_id_s[32];
    printf("Lihat Pengukuran Anak\n");
    printf("Masukkan ID Anak: "); fgets(child_id_s, sizeof(child_id_s), stdin); trim(child_id_s);
    long child_id = atol(child_id_s);
    if (child_id <= 0) { printf("ID tidak valid\n"); return; }

    FILE *f = fopen(MEAS_CSV, "r");
    if (!f) { printf("Belum ada data pengukuran (file measurements.csv tidak ditemukan).\n"); return; }
    printf("Pengukuran untuk Anak ID %ld:\n", child_id);
    printf("id | date       | age_mo | height_cm | zscore    | status\n");
    char *line;
    int any = 0;
    while ((line = read_line(f)) != NULL) {
        char *p = line;
        char *tok = next_token(&p, ','); long id = atol(tok); free(tok);
        tok = next_token(&p, ','); long cid = atol(tok); free(tok);
        tok = next_token(&p, ','); char *date = tok ? tok : strdup("");
        tok = next_token(&p, ','); int age = tok ? atoi(tok) : 0; if (tok) free(tok);
        tok = next_token(&p, ','); double height = tok ? atof(tok) : NAN; if (tok) free(tok);
        tok = next_token(&p, ','); char *weight = tok ? tok : strdup("");
        tok = next_token(&p, ','); char *zstr = tok ? tok : strdup("");
        tok = next_token(&p, ','); char *status = tok ? tok : strdup("");
        if (cid == child_id) {
            any = 1;
            printf("%4ld | %-10s | %6d | %8.2f | %8s | %s\n", id, date, age, height,
                   (strlen(zstr) ? zstr : "-"), status);
        }
        free(line);
    }
    fclose(f);
    if (!any) printf("Tidak ditemukan pengukuran untuk anak ini.\n");
}

/* simple statistics/summary */
static void summary_stats() {
    FILE *f = fopen(MEAS_CSV, "r");
    if (!f) { printf("Belum ada pengukuran.\n"); return; }
    int total = 0, normal = 0, stunted = 0, severe = 0, unknown = 0;
    char *line;
    while ((line = read_line(f)) != NULL) {
        char *p = line;
        char *tok = next_token(&p, ','); if (tok) { free(tok); }
        tok = next_token(&p, ','); if (tok) { free(tok); }
        tok = next_token(&p, ','); if (tok) { free(tok); }
        tok = next_token(&p, ','); if (tok) { free(tok); }
        tok = next_token(&p, ','); if (tok) { free(tok); }
        tok = next_token(&p, ','); if (tok) { free(tok); }
        tok = next_token(&p, ','); char *status = tok ? tok : strdup("");
        total++;
        if (strcmp(status, "Normal") == 0) normal++;
        else if (strcmp(status, "Stunted") == 0) stunted++;
        else if (strcmp(status, "Severely Stunted") == 0) severe++;
        else unknown++;
        free(line);
        if (status) free(status);
    }
    fclose(f);
    printf("Ringkasan pengukuran: total=%d, Normal=%d, Stunted=%d, Severe=%d, Unknown=%d\n",
           total, normal, stunted, severe, unknown);
}

/* ---------- Main loop ---------- */

int main(void) {
    WhoTable who;
    who_init(&who);
    printf("Stunting Data Recorder (C console)\n");
    printf("Memuat referensi WHO dari: %s\n", WHO_CSV_PATH);
    if (who_load(&who, WHO_CSV_PATH)) {
        printf("WHO reference loaded: %zu rows\n", who.n);
    } else {
        printf("WHO reference not found or gagal dimuat. Z-score disabled.\n");
    }

    ensure_data_dir_exists();

    while (1) {
        printf("\nMenu:\n");
        printf(" 1. Tambah Anak\n");
        printf(" 2. Tambah Pengukuran\n");
        printf(" 3. Daftar Anak\n");
        printf(" 4. Lihat Pengukuran Anak\n");
        printf(" 5. Ringkasan Statistik Pengukuran\n");
        printf(" 0. Keluar\n");
        printf("Pilih (0-5): ");
        char choice[8];
        if (!fgets(choice, sizeof(choice), stdin)) break;
        int c = atoi(choice);
        switch (c) {
            case 1: add_child(); break;
            case 2: add_measurement(&who); break;
            case 3: list_children(); break;
            case 4: view_measurements(); break;
            case 5: summary_stats(); break;
            case 0:
                printf("Sampai jumpa.\n");
                who_free(&who);
                return 0;
            default:
                printf("Pilihan tidak dikenal.\n");
        }
    }

    who_free(&who);
    return 0;
}


