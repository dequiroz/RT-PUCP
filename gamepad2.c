#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
//#define   test_bit(_n,_p)     !! ( _n & ( 1u << _p))

int main() {
    int fd = -1;
    const char *filename = "/dev/input/event0";
    char name[256]="Unknown";
    int yalv;

    //Opens device
    if ( (fd = open(filename, O_RDONLY) ) < 0) {
        perror("evdev open");
        return(1);
    }
    //prints name of device
    if(ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
        perror("event ioctl");
    }
    printf("The device on %s says its name is %s\n", filename, name);



    /* how many bytes were read */
    size_t rb;
    /* the events (up to 64 at once) */
    struct input_event ev[2];
    while(1) {
        rb=read(fd,ev,sizeof(struct input_event)*64);

//        printf("Data read ");
        if (rb < (int) sizeof(struct input_event)) {
            perror("evtest: short read");
            return(1);
        }
//        printf("rb size: %d \n",rb);

        for (yalv = 0; yalv < (int) (rb / sizeof(struct input_event)); yalv++) {
            //if ( (EV_ABS == ev[yalv].type)||(EV_KEY == ev[yalv].type) ) {
                printf("Event: time ");
                printf("%ld.%06ld, ",
                   ev[yalv].time.tv_sec,
                   ev[yalv].time.tv_usec);
                printf("type %d code %d value %d\n",
                   ev[yalv].type,
                   ev[yalv].code, ev[yalv].value);
            //}
        }
    }
}

