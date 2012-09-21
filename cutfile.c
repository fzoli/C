#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* a megadott szöveget a megadott fájlba írja */
rewrite_whole_file(const char *file_name, const char *text) {
    FILE *f;
    f = fopen(file_name, "w");
    if (f != NULL) {
        if (fputs(text, f) == EOF) {
            fprintf(stderr, "Could not write to %s: %s.\n", file_name, strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (fclose(f) != 0) {
            fprintf (stderr, "Error closing %s: %s.\n", file_name, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "Could not open %s to write: %s.\n", file_name, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    int argf = 1, state = 2; /* feltételezem, hogy csak a 2 fájl paraméter van megadva, és nem lesz találat */
    bool fast = false, verb = false; /* alapértelmezett érték definiálása */

    /* ha meg lettek adva paraméterek, alapértelmezettek felüldefiniálása */
    if (argc > 1 && !strcmp(argv[1], "-f")) fast = true;
    if (argc > 1 && !strcmp(argv[1], "-v")) verb = true;
    if (argc > 2 && !strcmp(argv[2], "-f")) fast = true;
    if (argc > 2 && !strcmp(argv[2], "-v")) verb = true;

    if (fast && verb) argf = 3; /* ha mindkét paramétert megadták, 3-mas indexben van a forrás fájl */
    if ((fast && !verb) || (!fast && verb)) argf = 2; /* ha csak egy paramétert adtak meg, 2-es indexben van a forrás fájl */

    if (argc != argf + 2) { /* ha nem paraméterezték jól, súgó megjelenítése és kilépés */
        fprintf(stderr, "Usage: [-v] [-f] source_file target_file\n\nOptions:\n -f  Remove only the first occurrence.\n -v  Print instead write back.\n\nExit codes:\n  0  There are occurrences.\n  1  There is an error like file not exists.\n  2  There is any occurrence.\n  3  This message appeared.\n");
        return 3;
    }

    unsigned char *ss, *st, *sub1, *sub2; /* mutatók definiálása */
    /* fájl tartalmának teljes beolvasása memóriába */
    /* ha az első paraméter -f, odébb csúsznak a paraméter indexek */
    ss = read_whole_file(argv[argf]);
    st = read_whole_file(argv[argf + 1]);

    do {
        /* ss-ben megkeresi az első pontos előfordulását st-nek és a memóriacímét sub1-be teszi, így a mutató a szöveg elejét nem tartalmazza */
        sub1 = strstr(ss, st);
        /* ha talált: a pontos találat feltétele, hogy a talált karakter az első karakter legyen, vagy a talált karakter előtt új sor legyen */
        while (sub1 != NULL && !(*ss == *sub1 || sub1[-1] == '\n')) {
            sub1 = strstr(sub1 + 1, st);
        }

        if (sub1) { /* ha van memóriacím, tehát van találat */
            state = 0; /* állapotkód OK-ra állítása jelezve, hogy volt minimum egy találat */
            /* az [első előfordulás után] + [keresett szöveg mérete] címmel arrébb lévő memóriacím sub2-be kerül, így a string végét kapjuk */
            sub2 = strstr(sub1 + 1, sub1 + strlen(st));
            /* a keresett szöveg helyére a string végét teszi plusz a string vége jelet is bele veszi (ezért van + 1 index növelés) */
            strncpy(sub1, sub1 + strlen(st), strlen(sub2) + 1);
            if (fast) break; /* ha gyorskeresést kértek, ciklusból kilépés, hogy több előfordulást ne keressen és ne vágjon */
        }
    } while (sub1 != NULL);

    if (verb) { /* ha a kimenetre kérik a feldolgozott tartalmat */
        /* megy a stdout kimenetre a feldolgozott tartalom */
        printf("%s", ss);
        fflush(stdout);
    }
    else {
        /* megy vissza a forrás fájlba a feldolgozott tartalom */
        rewrite_whole_file(argv[argf], ss);
    }

    /* fel kell szabadítani a memóriát, mert a read_whole_file metódus az malloc metódussal lefoglalta a memória egy részét, és kézzel kell szabaddá tenni */
    free(ss);
    free(st);

    return state; /* találattól függően 0 vagy 2 kóddal tér vissza */
}
