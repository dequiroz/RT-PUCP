#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdint.h>
#define   test_bit(_n,_p)     !! ( _n & ( 1u << _p))

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min)*(out_max - out_min) / (in_max - in_min) + out_min;
}
int main() {
    int fd = -1;
    const char *filename = "/dev/input/event0";
    char name[256]="Unknown";
    uint8_t yalv;

    int hor = 0;
    int ver = 0;
    int angle1 = 0;
    int angle2 = 0;

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


//    uint8_t abs_b[ABS_MAX/8+1];
//    for(yalv=0; yalv<ABS_MAX/8+1;yalv++)
//        abs_b[yalv]=0;
//    int abs_b;
    struct input_absinfo abs_feat;

//    ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(abs_b)), abs_b);
//    printf("Supported Absolute axes_ \n");

/*    for(yalv=0; yalv< ABS_MAX/8+1; yalv++) {
        printf("abs_b[%d]: 0x%x\n",yalv,abs_b[yalv]);
        if(ioctl(fd, EVIOCGABS(yalv), &abs_feat)) {
            perror("evdev EVIOCGABS ioctl");
        }
        printf("%d (min:%d, max:%d, flat:%d, fuzz:%d)",
               abs_feat.value,
               abs_feat.minimum,
               abs_feat.maximum,
               abs_feat.flat,
               abs_feat.fuzz);
    printf("\n");
    }*/
    while(1) {
        //X axis
        if(ioctl(fd, EVIOCGABS(0), &abs_feat)) {
            perror("evdev EVIOCGABS ioctl");
        }
        hor = abs_feat.value;
//        angle1 = (hor+32768)*(180-0)/(32767+32768)+0;
        angle1 = map(hor, abs_feat.min, abs_feat.max, 0, 180);
        //Y Axis
        if(ioctl(fd, EVIOCGABS(1), &abs_feat)) {
            perror("evdev EVIOCGABS ioctl");
        }
        ver = abs_feat.value;
//        angle2 = (ver+32768)*(180-0)/(32767+32768)+0;
        angle2 = map(ver, abs_feat.min, abs_feat.max, 0, 180);
        printf("Horizontal: %d - %d\t",hor, angle1);
        printf("Vertical: %d - %d\n", ver, angle2);
        sleep(1);
    }
}
