// https://github.com/linux-test-project/ltp/blob/master/testcases/kernel/syscalls/sysinfo/sysinfo03.c

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sched.h>
#include <sys/sysinfo.h>

static int offsets[] = {
	10,
	-10,
	3600,
};

static long read_proc_uptime(void)
{
    FILE *fp;
    double uptime, idletime;

    fp = fopen("/proc/uptime", "r");
    if (fp == NULL)
    {
        perror("Failed to open /proc/uptime");
        return 0;
    }
    if (fscanf(fp, "%lf %lf", &uptime, &idletime) != 2)
    {
        perror("Failed to read uptime and idletime");
        fclose(fp);
        return 0;
    }
    fclose(fp);

    printf("proc.uptime: uptime=%lf idletime=%lf\n", uptime, idletime);
    if ((long)(uptime * 1000) % 1000)
        return (long)ceil(uptime);
    return (long)uptime;
}

static void verify_sysinfo(unsigned int n)
{
    struct sysinfo si;
    long uptime;
    int off = offsets[n];

    unshare(CLONE_NEWTIME);

    // char *cmd[] = "echo "clockid off_ses off_nsec" > /proc/self/timens_offsets"
    // system("")

    sysinfo(&si);

    uptime = si.uptime;

    if (!fork())
    {
        sysinfo(&si);
        long proc_uptime = read_proc_uptime();

        long diff = si.uptime - uptime;

        printf("si.uptime=%ld uptime=%ld diff=%ld proc_uptime=%ld\n", si.uptime, uptime, diff, proc_uptime);

        if (diff < off || diff > off + 1)
            printf("TFAIL, Wrong sysinfo uptime offset %li \n", diff);
        else
            printf("TPASS, Correct sysinfo uptime offset %i \n", off);

        if (si.uptime < proc_uptime || si.uptime > proc_uptime + 1)
        {
            printf("TFAIL, /proc/uptime %li differs from sysinfo %li \n", proc_uptime, si.uptime);
        }
        else
        {
            printf("TPASS, /proc/uptime is consistent with sysinfo \n");
        }
    }
}

int main()
{
    int array_size = 1;//sizeof(offsets)/sizeof(offsets[0]);
    for (int i = 0; i < array_size; i++)
    {
        verify_sysinfo(i);
    }
    return 0;
}
