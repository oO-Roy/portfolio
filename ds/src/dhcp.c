/* 
	File: Dynamic Host Configuration Protocol.
	Status: APPROVED.
*/

#include <stdlib.h> /*malloc & free*/
#include <stdint.h> /*uint32_t*/
#include <string.h> /*memcpy, memcmp*/
#include <assert.h> /*assert*/

#include "dhcp.h"   /*typedef, size_t*/
#include "binary_trie.h"   /*Private API*/

#define SIZE_OF_IP 32
#define MAX_NUM_OF_BITS_TO_COPY 30

#define HOST_NUM_OF_BITS(dhcp) (SIZE_OF_IP - dhcp->num_of_bits_in_net_id)
#define SUBNET_MASK(dhcp) ((~0) << HOST_NUM_OF_BITS(dhcp))

struct dhcp
{
    dhcp_ip net_id;
    size_t num_of_bits_in_net_id;
    trie_node_t* root;
};

/*----------------------------------------------------------------------------*/
static trie_node_t* GetRoot(dhcp_t* dhcp)
{
    assert(dhcp);

    return dhcp->root;
}

static size_t GetNumOfBitsInNetId(dhcp_t* dhcp)
{
    assert(dhcp);

    return dhcp->num_of_bits_in_net_id;
}

static int IsNumOfBitsToCopyValid(size_t num_of_bits_in_net_id)
{
    return (0 < num_of_bits_in_net_id 
                        && MAX_NUM_OF_BITS_TO_COPY >= num_of_bits_in_net_id);
}

static void CpySrcEndToDestStart(dhcp_ip dest, dhcp_ip src)
{
    int i = (BYTES_IN_IP - 1);
    int j = 0;

    for (; i >= 0; --i, ++j)
    {
        dest[j] = src[i];
    }
}

static int IsReqIDValid(dhcp_t* dhcp ,dhcp_ip req_ip)
{
    dhcp_ip req = {0};
    dhcp_ip net = {0};

    assert(dhcp);    
    
    CpySrcEndToDestStart(req, req_ip);
    CpySrcEndToDestStart(net, dhcp->net_id);

    *(uint32_t*)req &= SUBNET_MASK(dhcp);
    *(uint32_t*)net &= SUBNET_MASK(dhcp);

    return (*(uint32_t*)req == *(uint32_t*)net);
}

static void SetDHCPEntities(dhcp_t* dhcp)
{
    dhcp_ip req_id = {0};
    uint32_t path = 0;

    assert(dhcp);

    GetNextNode(GetRoot(dhcp), path, req_id);
    
    path = ((1 << HOST_NUM_OF_BITS(dhcp)) - 1);
    path <<= GetNumOfBitsInNetId(dhcp);
    GetNextNode(GetRoot(dhcp), path, req_id);

    path = ((1 << HOST_NUM_OF_BITS(dhcp)) - 2);
    path <<= GetNumOfBitsInNetId(dhcp);
    GetNextNode(GetRoot(dhcp), path, req_id);
}

/*----------------------------------------------------------------------------*/
dhcp_t* DHCPCreate(dhcp_ip net_id, size_t num_of_bits_in_net_id)
{
    dhcp_t* dhcp = NULL;
    
    if (!IsNumOfBitsToCopyValid(num_of_bits_in_net_id))   
    {
        return NULL;
    }
    
    dhcp = malloc(sizeof(dhcp_t));
    if (NULL == dhcp)
    {
        return NULL;
    }

    dhcp->root = BTrieCreate((SIZE_OF_IP - num_of_bits_in_net_id));
    if (NULL == GetRoot(dhcp))
    {
        free(dhcp);
        return NULL;
    }

    memcpy(dhcp->net_id, net_id, BYTES_IN_IP);
    dhcp->num_of_bits_in_net_id = num_of_bits_in_net_id;

    SetDHCPEntities(dhcp);

    return dhcp;
}

void DHCPDestroy(dhcp_t* dhcp)
{
    if (NULL != dhcp)
    {
        BTrieDestroy(GetRoot(dhcp));
    }
    
    free(dhcp);
}

dhcp_status_t DHCPLease(dhcp_t* dhcp, dhcp_ip req_ip, dhcp_ip alloc_ip)
{
    dhcp_ip path = {0};
    dhcp_ip tmp = {0};
    
    if (!dhcp)
    {
        *(uint32_t*)alloc_ip = 0;
        return INVALID_INPUT;
    }
    
    if (!IsReqIDValid(dhcp, req_ip))
    {
        *(uint32_t*)alloc_ip = 0;
        return INVALID_IP_ADDRESS;
    }

    CpySrcEndToDestStart(path, req_ip);

    /*Copying first part of IP*/
    memcpy(tmp, path, BYTES_IN_IP);

    *(uint32_t*)path <<= GetNumOfBitsInNetId(dhcp);

    /*Setting tmp to get new bits*/
    *(uint32_t*)tmp >>= HOST_NUM_OF_BITS(dhcp);

    if (!GetNextNode(GetRoot(dhcp), *(uint32_t*)path, tmp))
    {
        *(uint32_t*)alloc_ip = 0;
        return NO_FREE_IP_ADDRESS;
    }
    
    CpySrcEndToDestStart(alloc_ip, tmp);

    if (memcmp(req_ip, alloc_ip, BYTES_IN_IP))
    {
        return LEASED_ANOTHER_IP;
    }

    return SUCCESS;
}

dhcp_status_t DHCPRelease(dhcp_t* dhcp, dhcp_ip ip_to_release)
{
    dhcp_ip path = {0};
    
    if (NULL == dhcp)
    {
        return INVALID_INPUT;
    }

    if (!IsReqIDValid(dhcp, ip_to_release))
    {
        return INVALID_FREE;
    }
    
    CpySrcEndToDestStart(path, ip_to_release);
    
    return (FreeLeaf(GetRoot(dhcp)
                , (*(uint32_t*)path << GetNumOfBitsInNetId(dhcp))) 
                                                    ? DOUBLE_FREE : SUCCESS);
}

size_t DHCPCountFree(const dhcp_t* dhcp)
{
    if (NULL == dhcp)
    {
        return 0;
    }

    return GetAmountOfFreeLeaves(GetRoot((dhcp_t*)dhcp));    
}