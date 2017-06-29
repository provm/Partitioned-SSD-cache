#include <stdio.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

// call this function to start a nanosecond-resolution timer
struct timespec timer_start(){
    struct timespec start_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
    return start_time;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time){
    struct timespec end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    long diffInNanos = end_time.tv_nsec - start_time.tv_nsec;
    return diffInNanos;
}

void do_computation(int n) {
        usleep(n);
}

int main (void)
{
        const gsl_rng_type * T;
        gsl_rng * r;

        int i, n = 1000000;
        double mu = 50000;
        /* create a generator chosen by the 
        environment variable GSL_RNG_TYPE */

	gsl_rng_env_setup();
        T = gsl_rng_default;
        r = gsl_rng_alloc (T);

        char buf[4096];
        int ret = 0;
        int fd = open("10Gb.dat", O_RDONLY, 0);
        if(fd < 0)
                printf("error in file open\n");
        int count = 5;
        while(count-- ) {
                for (i = 0; i < n; i++)
                {
                        unsigned int k = gsl_ran_gaussian (r, mu);
			k = abs(k) * 4096;
//                      do_computation(1000);
                        ret = lseek(fd, k, SEEK_SET);
                        if(ret < 0)
                              printf("lseeek error: %d\n",k);
                       struct timespec vartime = timer_start();
			read(fd, &buf, sizeof(buf));
			long time_elapsed = timer_end(vartime);
			printf("%ld \n", time_elapsed);
                        }
                }
        }
        gsl_rng_free (r);
        close(fd);
        return 0;
}
