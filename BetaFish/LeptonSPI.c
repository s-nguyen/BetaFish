#include "LeptonSPI.h"




static void pabort(const char *s)
{
    perror(s);
    abort();
}

int initCamera() {
    fd = open(device, O_RDWR);
        if (fd < 0)
            pabort("can't open device");
        else if (-1 == ioctl(fd, SPI_IOC_WR_MODE, &mode))
            pabort("can't set spi mode");
        else if (-1 == ioctl(fd, SPI_IOC_RD_MODE, &mode))
            pabort("can't get spi mode");
        else if (-1 == ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits))
            pabort("can't set bits per word");
        else if (-1 == ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits))
            pabort("can't get bits per word");
        else if (-1 == ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed))
            pabort("can't set max speed hz");
        else if (-1 == ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed))
            pabort("can't get max speed hz");
        else
            return 1;
        return 0;

}

int closeCamera()
{
    int status_value = -1;


    status_value = close(fd);
    if(status_value < 0)
    {
        perror("Error - Could not close SPI device");
        exit(1);
    }
    return(status_value);
}



void getFrame() {
        int ret;
        int i;
        int frame_number;
        int packetRow;
        int resets = 0;
        uint8_t tx[VOSPI_FRAME_SIZE] = {0, };

        struct spi_ioc_transfer tr = {
                .tx_buf = (unsigned long)tx,
                .rx_buf = (unsigned long)lepton_frame_packet,
                .len = VOSPI_FRAME_SIZE,
                .delay_usecs = delay,
                .speed_hz = speed,
                .bits_per_word = bits,
        };

        while(frame_number != 59) {
            ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
            if (ret < 1)
                pabort("can't send spi message");
            if((lepton_frame_packet[0]&0xf) != 0x0f) {
               frame_number = lepton_frame_packet[1];
               packetRow = frame_number*PACKET_SIZE;
               if(frame_number < 60) {
                   for(i = 0; i < PACKET_SIZE; i++) {
                        frame[i+packetRow] = lepton_frame_packet[i];
                   }
               }
            }
            else {
                resets = resets + 1;
                usleep(1000);
                if(resets == 750) {
                    closeCamera();
                    usleep(750000);
                    initCamera();
                }
            }
        }
        printf("jobs done\n");

}


