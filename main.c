#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "thread.h"
#include "ztimer.h"
#include "shell.h"

#include "mutex.h"
#include <arpa/inet.h>

#include "fmt.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/utils.h"
/* Add lsm303dlhc related include here */
#include "lsm303dlhc.h"
#include "lsm303dlhc_params.h"

/* Declare the lsm303dlhc device variable here */
static lsm303dlhc_t lsm303dlhc;

/* Declare and initialize the lsm303dlhc thread lock here */
static mutex_t lsm_lock = MUTEX_INIT_LOCKED;

/* stack memory allocated for the lsm303dlhc thread */
static char lsm303dlhc_stack[THREAD_STACKSIZE_MAIN];

void send_accelerometer_data(lsm303dlhc_3d_data_t acc_value) {
    char address[IPV6_ADDR_MAX_STR_LEN] = "2600:1900:4150:a91b::";
    uint16_t port = 8683;
    ipv6_addr_t addr; 
    uint8_t payload[128]; // Adjust payload buffer size as needed
    uint8_t buf[CONFIG_GCOAP_PDU_BUF_SIZE];
    sock_udp_ep_t remote = SOCK_IPV6_EP_ANY;
    coap_pkt_t pdu;
    size_t payload_len = snprintf((char *)payload, sizeof(payload),
                                  "x: %i y: %i z: %i\n",
                                  acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);

    gcoap_req_init(&pdu, &buf[0], CONFIG_GCOAP_PDU_BUF_SIZE, 1, "/whoami");
    // Copy payload into CoAP packet
    coap_hdr_set_type(pdu.hdr, 1);
    memcpy(pdu.payload, payload, payload_len);
    pdu.payload_len = payload_len;

    // Finish the CoAP request
    coap_opt_finish(&pdu, COAP_OPT_FINISH_NONE);

    remote.family = AF_INET6;
    remote.port   = port;
    //remote.addr.ipv6   = addr;
    ipv6_addr_from_str(&addr, address);
    memcpy(&remote.addr.ipv6[0], &addr.u8[0], sizeof(addr.u8));
    //sock_udp_name2ep(&remote, address);
    //new_remote = &remote;
    // Send CoAP request        
    printf("gcoap_cli: sending msg ID %u, %u bytes\n", coap_get_id(&pdu),
               (unsigned) payload_len);
    printf("Port: %x",*remote.addr.ipv6);
    gcoap_req_send(&buf[0],payload_len,&remote,NULL, NULL);

    printf("CoAP POST request sent successfully!\n");

}

static void *lsm303dlhc_thread(void *arg)
{
    (void)arg;
    //char addr[24]     = "[2600:1900:4150:a91b::]";
    while (1) {
        /* Acquire the mutex here */
        mutex_lock(&lsm_lock);

        /* Read the accelerometer/magnetometer values here */
        lsm303dlhc_3d_data_t mag_value;
        lsm303dlhc_3d_data_t acc_value;
        lsm303dlhc_read_acc(&lsm303dlhc, &acc_value);
        printf("Accelerometer x: %i y: %i z: %i\n",
               acc_value.x_axis, acc_value.y_axis, acc_value.z_axis);
        lsm303dlhc_read_mag(&lsm303dlhc, &mag_value);
        printf("Magnetometer x: %i y: %i z: %i\n",
               mag_value.x_axis, mag_value.y_axis, mag_value.z_axis);
        
        /* Release the mutex here */
        mutex_unlock(&lsm_lock);


        send_accelerometer_data(acc_value);


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
    /* lsm303dlhc shell command handler */
    { "lsm", "start/stop reading accelerometer values", lsm303dlhc_handler },
    
};

int main(void)
{
    /* Initialize the lps331ap sensor here */


    /* Initialize the lsm303dlhc sensor here */
    lsm303dlhc_init(&lsm303dlhc, &lsm303dlhc_params[0]);

    thread_create(lsm303dlhc_stack, sizeof(lsm303dlhc_stack), THREAD_PRIORITY_MAIN - 1,
                  0, lsm303dlhc_thread, NULL, "lsm303dlhc");

    /* Everything is ready, let's start the shell now */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(commands, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}