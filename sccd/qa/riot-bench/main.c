#include <stdio.h>
#include <sccd/qa/bench.c>

int main(void) {
    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    bench();
    return 0;
}
