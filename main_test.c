#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "ztimer.h"
#include "shell.h"
#include "mutex.h"
#include "lsm303dlhc.h"
#include "lsm303dlhc_params.h"
#include "net/sock/udp.h"
#include "coap.h"

/* lsm303dlhc device variable */
static lsm303dlhc_t lsm303dlhc;

/* lsm303dlhc thread lock */
static mutex_t lsm_lock = MUTEX_INIT_LOCKED;

/* stack memory allocated for the lsm303dlhc thread */
static char lsm303dlhc_stack[THREAD_STACKSIZE_MAIN];

/* COAP variables */
static coap_pkt_t pkt;
static sock_udp_ep_t remote;

/* COAP server IP address and port */
#define SERVER_IP "::1" // Replace with your COAP server IP address
#define SERVER_PORT 5683 // Replace with your COAP server port

/* Function to send sensor data via COAP */
static void send_sensor_data(int16_t acc_x, int16_t acc_y, int16_t acc_z,
                             int16_t mag_x, int16_t mag_y, int16_t mag_z) {
    /* Initialize COAP packet */
    coap_init_pkt(&pkt, COAP_TYPE_CON, COAP_METHOD_POST, coap_get_mid());
    
    /* Create payload */
    char payload[128]; // Adjust the size according to your data
    snprintf(payload, sizeof(payload), "Accelerometer: x=%i y=%i z=%i, Magnetometer: x=%i y=%i z=%i",
             acc_x, acc_y, acc_z, mag_x, mag_y, mag_z);
    pkt.payload = (uint8_t *)payload;
    pkt.payload_len = strlen(payload);

    /* Set COAP remote address */
    remote.family = AF_INET6;
    remote.port = SERVER_PORT;
    ipv6_addr_from_str((ipv6_addr_t *)&remote.addr, SERVER_IP);

    /* Send COAP packet */
    sock_udp_send(NULL, pkt.payload, pkt.payload_len, &remote);
}

static void *lsm303dlhc_thread(void *arg) {
    (void)arg;

    while (1) {
        /* Acquire the mutex */
        mutex_lock(&lsm_lock);

        /* Read the accelerometer/magnetometer values */
        lsm303dlhc_3d_data_t mag_value;
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
        lsm303dlhc_read_mag(&lsm303dlhc, &mag_value);

        /* Send sensor data via COAP */
        send_sensor_data(acc_value.x_axis, acc_value.y_axis, acc_value.z_axis,
                         mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);
        
        /* Release the mutex */
        mutex_unlock(&lsm_lock);

        ztimer_sleep(ZTIMER_MSEC, 1000);
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

    /* Implement the lsm303dlhc start/stop subcommands */
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
    /* lsm303dlhc shell command handler */
    { "lsm", "start/stop reading accelerometer values", lsm303dlhc_handler },

};

int main(void)
{
    /* Initialize the lsm303dlhc sensor */
    lsm303dlhc_init(&lsm303dlhc, &lsm303dlhc_params[0]);

    thread_create(lsm303dlhc_stack, sizeof(lsm303dlhc_stack), THREAD_PRIORITY_MAIN - 1,
                  0, lsm303dlhc_thread, NULL, "lsm303dlhc");

    /* Everything ready, starting shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}