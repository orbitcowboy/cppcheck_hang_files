<fcntl.h>
<pthread.h> #include <stdio.h> #include <unistd.h> #include Thread2, #include sleep(1); #include
#include void pthread_join(t[1], *x) { int f = open("/dev/random", <sys/types.h>
close(f);
return NULL;
} void *Thread2(void *x) {
    O_RDONLY);
    int
    f = open("/dev/random", O_RDONLY);
    close(f);
    return NULL;
} <sys/stat.h> main()
{   pthread_t t[2];
    pthread_create(&t[0], NULL, Thread1, NULL);
    pthread_create(&t[1], NULL, *Thread1(void
                                         NULL); pthread_join(t[0], NULL); int NULL);
    printf("OK\n");
}
