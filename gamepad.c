#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#define   test_bit(_n,_p)     !! ( _n & ( 1u << _p))

main() {

    int fd = -1;
    const char *filename = "/dev/input/event0";

    char name[256]= "Unknown";
    int evtype_b;

    if ( (fd = open(filename, O_RDONLY) ) < 0) {
        perror("evdev open");
        return(1);
    }
//    printf("yay\n");

    if(ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
        perror("event ioctl");
    }
    printf("The device on %s says its name is %s\n", filename, name);


//    evtype_b;
//    memset(evtype_b, 0, sizeof(evtype_b));

    if(ioctl(fd, EVIOCGBIT(0,EV_MAX), evtype_b) < 0) {
        perror("event ioctl");
    }
    printf("Supported event types:\n");
    int yalv;
    for (yalv = 0; yalv < EV_MAX; yalv++) {
        if (test_bit(yalv, evtype_b)) {
            /* the bit is set in the event types list */
            printf("  Event type 0x%02x ", yalv);
            switch ( yalv) {
                case EV_SYN :
                    printf(" (Synch Events)\n");
                    break;
                case EV_KEY :
                    printf(" (Keys or Buttons)\n");
                    break;
                case EV_REL :
                    printf(" (Relative Axes)\n");
                    break;
                case EV_ABS :
                    printf(" (Absolute Axes)\n");
                    break;
                case EV_MSC :
                    printf(" (Miscellaneous)\n");
                    break;
                case EV_LED :
                    printf(" (LEDs)\n");
                    break;
                case EV_SND :
                    printf(" (Sounds)\n");
                    break;
                case EV_REP :
                    printf(" (Repeat)\n");
                    break;
                case EV_FF :
                case EV_FF_STATUS:
                    printf(" (Force Feedback)\n");
                    break;
                case EV_PWR:
                    printf(" (Power Management)\n");
                    break;
                default:
                    printf(" (Unknown: 0x%04hx)\n",yalv);
            }
        }
    }

    close(fd);

}
