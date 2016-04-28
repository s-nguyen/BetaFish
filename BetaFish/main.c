#include <stdio.h>
#include "LeptonSPI.h"
#include "TCP_Server.h"

int main() {
    uint8_t ack[1];
    initCamera();
    tcp_init();
    printf("Connected\n");

    while(1) {
        getFrame();
        printf("Recieved Frame");
        tcp_write(frame, 9840);
        tcp_read(ack, 1);
    }


    closeServer();
    return 0;
}
// system("modprobe g_serial");
