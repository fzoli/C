#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

/* csak linux rendszeren működőképes metódus */
static unsigned get_file_size(const char *file_name) {
    struct stat sb;
    if (stat(file_name, &sb) != 0) {
        fprintf (stderr, "%s: %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return sb.st_size;
}

/* lefoglalja a memóriát és beolvassa az egész fájlt */
static unsigned char *read_whole_file(const char *file_name) {
    unsigned s;
    unsigned char *contents;
    FILE *f;
    size_t bytes_read;
    int status;

    s = get_file_size(file_name);
    contents = malloc(s + 1);
    if (!contents) {
        fprintf(stderr, "Not enough memory.\n");
        exit(EXIT_FAILURE);
    }

    f = fopen(file_name, "r");
    if (!f) {
        fprintf(stderr, "Could not open %s: %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    bytes_read = fread(contents, sizeof(unsigned char), s, f);
    if (bytes_read != s) {
        fprintf(stderr, "Short read of %s: expected %d bytes but got %d: %s.\n", file_name, s, bytes_read, strerror(errno));
        exit(EXIT_FAILURE);
    }
    status = fclose(f);
    if (status != 0) {
        fprintf (stderr, "Error closing %s: %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return contents;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        fprintf(stderr, "Usage: source_file target_file\n");
        return 3; /* nem paraméterezték jól */
    }

    int state = 0; /* feltételezem, hogy lesz találat */
    unsigned char *ss, *st, *sub1, *sub2; /* mutatók definiálása */

    /* fájl tartalmának teljes beolvasása memóriába */
    ss = read_whole_file(argv[1]);
    st = read_whole_file(argv[2]);

    /* ss-ben megkeresi az első pontos előfordulását st-nek és a memóriacímét sub1-be teszi, így a mutató a szöveg elejét nem tartalmazza */
    sub1 = strstr(ss, st);

    /* ha talált: a pontos találat feltétele, hogy a talált karakter az első karakter legyen, vagy a talált karakter előtt új sor legyen */
    while (sub1 != NULL && !(*ss == *sub1 || sub1[-1] == '\n')) {
        sub1 = strstr(sub1 + 1, st);
    }

    if (sub1) { /* ha van memóriacím, tehát van találat */
        /* az [első előfordulás után] + [keresett szöveg mérete] címmel arrébb lévő memóriacím sub2-be kerül, így a string végét kapjuk */
        sub2 = strstr(sub1 + 1, sub1 + strlen(st));
        /* a keresett szöveg helyére a string végét teszi plusz a string vége jelet is bele veszi (ezért van + 1 index növelés) */
        strncpy(sub1, sub1 + strlen(st), strlen(sub2) + 1);
    }
    else { /* ha nincs találat, nincs vágás */
        state = 2;
    }

    /* megy a stdout kimenetre a feldolgozott tartalom, a többi már a bash dolga :D */
    printf("%s", ss);

    /* fel kell szabadítani a memóriát, mert a read_whole_file metódus az malloc metódussal lefoglalta a memória egy részét, és kézzel kell szabaddá tenni */
    free(ss);
    free(st);

    /* még mielőtt kilép a program, hogy bash-ben menjen a pipe, flush kell */
    fflush(stdout);

    return state; /* találattól függően 0 vagy 2 kóddal tér vissza */
}