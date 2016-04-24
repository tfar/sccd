#include <stdio.h>
#include <ps.h>

#include <sccd/qa/test.c>

int main(void) {
    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    test();
    return 0;
}
