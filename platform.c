#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <stdint.h>

#include <time.h>
#include <wiringPi.h>
#include <pthread.h>
#include <sched.h>

struct timespec on, off;

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min)*(out_max - out_min) / (in_max - in_min) + out_min;
}

void *servo(void *arg) {
   while(1) {
      //freq = 20 000 000 nsec
      digitalWrite(9,HIGH);
      nanosleep(&on, (struct timespec *)NULL);
      digitalWrite(9,LOW);
      nanosleep(&off, (struct timespec *)NULL);
   }
}

void *update(void *arg) {
   int i = 0;
   while(1) {
      printf("hi\n");
      if (i==0) {
         i++;
         on.tv_nsec = 1000000L;
         off.tv_nsec=19000000L;
      } else {
         i=0;
         on.tv_nsec =10000000;
         off.tv_nsec=10000000;
      }
      nanosleep( (const struct timespec[]){{1,0000000}},NULL );
   }
}

void *gamepad(void *arg) {
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
      pthread_exit(NULL);
   }
   //Prints name of device
   if(ioctl(fd, EVIOCGNAME(sizeof(name)), name) < 0) {
      perror("event ioctl");
      pthread_exit(NULL);
   }
   printf("The device on %s says its name is %s\n", filename, name);

   struct input_absinfo abs_feat;
   while(1) {
      //X axis
      if(ioctl(fd, EVIOCGABS(0), &abs_feat)) {
         perror("evdev EVIOCGABS ioctl");
      }
      hor = abs_feat.value;
      //Y Axis
      if(ioctl(fd, EVIOCGABS(1), &abs_feat)) {
         perror("evdev EVIOCGABS ioctl");
      }
      ver = abs_feat.value;
      //update values
      angle1 = map(hor, abs_feat.minimum, abs_feat.maximum, 0, 180);
      angle2 = map(ver, abs_feat.minimum, abs_feat.maximum, 0, 180);
      printf("Horizontal: %d - %d\t",hor, angle1);
      printf("Vertical: %d - %d\n", ver, angle2);
      sleep(1);
   }
}

int main(void) {
   on.tv_sec = 0;
   on.tv_nsec = 0;
   off.tv_sec = 0;

   pthread_t ser;
   pthread_t upd;
   pthread_t gp;

   //PRIORITY
   pthread_attr_t attr;
   struct sched_param param;

   param.sched_priority = 5;
   pthread_attr_init(&attr);
   pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
   pthread_attr_setschedparam(&attr,&param);

   wiringPiSetup();
   pinMode(9,OUTPUT);
   digitalWrite(9,LOW);

   pthread_create(&upd, &attr, update, NULL);
   pthread_create(&ser, &attr, servo,  NULL);
   param.sched_priority = 3;
   pthread_attr_setschedparam(&attr,&param);
   pthread_create(&gp , &attr, gamepad,NULL);

 //  pthread_

   pthread_exit(NULL);
}
