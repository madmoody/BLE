#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
//#include <bluetooth/bluez/attrib/gatt.h>


#define CONNECT_TIMEOUT 60 

char *MAC_ACCADD  = "12:34:56:78:90:00";
char *characteristic_uuid = "0x0112233445566778899AABBCCDDEEFFO";
char devicename[250] = {0};

int main() {
    int BLE_id = hci_get_route(NULL);
    int sock = hci_open_dev(BLE_id);

    if (sock < 0) {
        perror("HCI device open failed");
        exit(1);
    }

    
    inquiry_info* ii = NULL;
    int max_rsp = 255;
    int num_rsp = 0;
    long flags = IREQ_CACHE_FLUSH;
    ii = (inquiry_info*)malloc(max_rsp * sizeof(inquiry_info));


    bdaddr_t peer_bdaddr;
    str2ba("12:34:56:78:90:00", &peer_bdaddr);

    uint16_t handle = 0;
    uint8_t initiator_filter = 0;
    uint8_t peer_bdaddr_type = LE_PUBLIC_ADDRESS;
    uint8_t own_bdaddr_type = LE_PUBLIC_ADDRESS;
    uint16_t interval = htobs(0x0006);
    uint16_t window = htobs(0x0006);   
    uint16_t min_interval = 0x0006;    
    uint16_t max_interval = 0x0C80;    
    uint16_t latency = 0;
    uint16_t supervision_timeout = 0x0640;
    uint16_t min_ce_length = 0x0000;  
    uint16_t max_ce_length = 0xFFFF;  

    num_rsp = hci_inquiry(BLE_id, 8, max_rsp, NULL, &ii, flags);
    if (num_rsp < 0) {
        perror("HCI inquiry failed");
        close(sock);
        exit(1);
    }
    
 char BLEADD[255];
 char BLELOCAL[255];
	for(int i = 0;i<num_rsp;i++){
	ba2str(&ii[i].bdaddr,BLEADD);
	

	printf("Device found %s\n:",BLEADD);
	memset(devicename, 0, sizeof(devicename));

	int read = hci_read_remote_name(sock,&ii[i].bdaddr,sizeof(devicename),devicename,0);
        memset(BLELOCAL, 0, sizeof(BLELOCAL));

        int Local_Name = hci_read_local_name(sock,8,BLELOCAL,0);
	
	if(read < 0){
	perror("Not able read:"); 
	exit(1);
	}
	if(Local_Name < 0){
	perror("Not able to read Local");
	exit(1);	
	}
	
	printf("Device name: %s\n local Name: %s\n", devicename,BLELOCAL);
	}

	int r = hci_le_create_conn(sock, interval, window, initiator_filter, peer_bdaddr_type,
        peer_bdaddr, own_bdaddr_type, min_interval, max_interval, latency,
        supervision_timeout, min_ce_length, max_ce_length, &handle, CONNECT_TIMEOUT * 1000000);

        if (r < 0) {
        perror("Failed to create connection");
	exit(1);

        } 
	
        printf("Handle: %d (0x%04x)\n", handle, handle);
       

    free(ii);
    close(sock);
    return 0;
}

