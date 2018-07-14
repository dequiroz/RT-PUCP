#include <wiringPi.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

struct timespec on, off;
long total = 10000000L;

void *blink(void *arg) {
   while(1) {
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

void *servo(void *arg) {
   while(1) {
      //freq = 20 000 000 nsec
      digitalWrite(9,HIGH);
      nanosleep(&on, (struct timespec *)NULL);
      digitalWrite(9,LOW);
      nanosleep(&off, (struct timespec *)NULL);
   }
}

int main(void) {
   on.tv_sec = 0;
   on.tv_nsec = 0;
   off.tv_sec = 0;
   off.tv_nsec = total;

   pthread_t ser;
   pthread_t upd;

   wiringPiSetup ();
   pinMode(9,OUTPUT);
   digitalWrite(9,HIGH);

   pthread_create(&upd,NULL,update,NULL);
   pthread_create(&ser,NULL,servo,NULL);
   pthread_exit(NULL);
   return 0;
}
