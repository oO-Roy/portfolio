#ifndef __DHCP_H__
#define __DHCP_H__

#include <stddef.h> /*size_t*/

#define BYTES_IN_IP 4

typedef unsigned char dhcp_ip[BYTES_IN_IP]; 

typedef enum
{
    SUCCESS = 0,
    NO_FREE_IP_ADDRESS = 1, 
    INVALID_IP_ADDRESS = 2,
    ALLOCATION_FAIL = 3,
    DOUBLE_FREE = 4,
    INVALID_INPUT = 5,
    INVALID_FREE = 6,
    LEASED_ANOTHER_IP = 7
}dhcp_status_t;

typedef struct dhcp dhcp_t;

dhcp_t* DHCPCreate(dhcp_ip net_id, size_t num_of_bits_in_net_id);

void DHCPDestroy(dhcp_t* dhcp);

/* alloc_ip is the outparam */
dhcp_status_t DHCPLease(dhcp_t* dhcp, dhcp_ip req_ip, dhcp_ip alloc_ip);

dhcp_status_t DHCPRelease(dhcp_t* dhcp, dhcp_ip ip_to_release);

size_t DHCPCountFree(const dhcp_t* dhcp);

#endif /*__DHCP_H__*/ 
