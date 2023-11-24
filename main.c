#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "ztimer.h"
#include "shell.h"

#include "mutex.h"
#include <arpa/inet.h>

#include "fmt.h"
#include "net/sock/util.h"
#include "net/utils.h"
#include "lsm303dlhc.h"
#include "lsm303dlhc_params.h"
#include "od.h"

static lsm303dlhc_t lsm303dlhc;

/* Declare and initialize the lsm303dlhc thread lock here */
static mutex_t lsm_lock = MUTEX_INIT_LOCKED;

static char lsm303dlhc_stack[THREAD_STACKSIZE_MAIN];

static void *lsm303dlhc_thread(void *arg)
{
    (void)arg;
    char server_address[IPV6_ADDR_MAX_STR_LEN] = "[2600:1900:4150:a91b::]:8683";

    char sensor_data[128];
    sock_udp_ep_t remote;
    remote.family = AF_INET6;

    if (sock_udp_str2ep(&remote, server_address) != 0) {
        printf("Invalid address or port\n");
        return 0;
    }
    while (1) {
        /* Acquire the mutex here */
        mutex_lock(&lsm_lock);

        /* Read the accelerometer/magnetometer values here */
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
        printf("Accelerometer x: %i y: %i z: %i\n",
               acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
        
        /* Release the mutex here */
        mutex_unlock(&lsm_lock);

        snprintf(sensor_data, sizeof(sensor_data), "Accelerometer x: %i y: %i z: %i\n",
             acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);

        ssize_t bytes_sent = sock_udp_send(NULL, sensor_data, strlen(sensor_data), &remote);
        if (bytes_sent < 0) {
            printf("Failed to send data\n");
        
        } else {
            printf("Sensor data sent to the server\n");
        }

        //5 seconds of sleep
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}

static void _lsm303dlhc_usage(char *cmd)
{
    printf("usage: %s <start|stop>\n", cmd);
}

static int lsm303dlhc_handler(int argc, char *argv[])
{
    if (argc < 2) {
        _lsm303dlhc_usage(argv[0]);
        return -1;
    }
    /* Implement the lsm303dlhc start/stop subcommands here */
    if (!strcmp(argv[1], "start")) {
        mutex_unlock(&lsm_lock);
    }
    else if (!strcmp(argv[1], "stop")) {
        mutex_trylock(&lsm_lock);
    }
    else {
        _lsm303dlhc_usage(argv[0]);
        return -1;
    }
    return 0;
}

static const shell_command_t commands[] = {
    { "lsm", "start/stop reading accelerometer values", lsm303dlhc_handler },
    
};

int main(void)
{
    lsm303dlhc_init(&lsm303dlhc, &lsm303dlhc_params[0]);
    thread_create(lsm303dlhc_stack, sizeof(lsm303dlhc_stack), THREAD_PRIORITY_MAIN - 1,
                  0, lsm303dlhc_thread, NULL, "lsm303dlhc");

    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);
    return 0;
}