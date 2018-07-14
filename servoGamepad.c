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

//    sleep(3);
    printf("Reading\n");
    /* how many bytes were read */
    size_t rb;
    /* the events (up to 64 at once) */
    struct input_event events[64];
    struct input_event ev;

    int hor = 0;
    int ver = 0;
//    struct input_event *ev;
    while(1) {
//        sleep(1);
        rb=read(fd,events,sizeof(struct input_event)*64);
        printf("rb size: %d\n", rb);

        if (rb < (int) sizeof(struct input_event)) {
            perror("evtest: short read");
            return(1);
        }
        printf("index: %d \n", rb / sizeof(struct input_event)-2);
//        ev = events[0];
        ev = events[ rb/sizeof(struct input_event)-2 ];
        if ( (EV_ABS == ev.type)||(EV_KEY == ev.type) ) {
            switch(ev.code) {
                case 0: //horizontal
                    hor = ev.value;
                    printf("Horizontal: %d\n",hor);
                    break;
                case 1: //vertical
                    ver = ev.value;
                    printf("Vertical: %d\n", ver);
                    break;
            }
/*            printf("Event: time ");
            printf("%ld.%06ld, ",
               ev.time.tv_sec,
               ev.time.tv_usec);
            printf("type %d code %d value %d\n",
               ev.type,
               ev.code, ev.value);
*/
        }
    }
}

