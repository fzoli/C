#include <stdlib.h>

#ifdef unix
    const char* clr_cmd = "clear";
#else
    const char* clr_cmd = "cls";
#endif

void clrscr() {
    system(clr_cmd);
}

