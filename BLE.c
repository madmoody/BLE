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
    uint16_t ptype = 0x0800;   
    uint16_t clkoffset = 0x0000; 
    uint8_t rswitch = 0x01;  

    num_rsp = hci_inquiry(BLE_id, 8, max_rsp, NULL, &ii, flags);
    if (num_rsp < 0) {
        perror("HCI inquiry failed");
        close(sock);
        exit(1);
    }
    
 char BLEADD[255];
 char BLELOCAL[255];
 char BLEFEATURE[255];
	for(int i = 0;i<num_rsp;i++){
	ba2str(&ii[i].bdaddr,BLEADD);
	

	printf("Device found %s\n:",BLEADD);
	memset(devicename, 0, sizeof(devicename));

	int read = hci_read_remote_name(sock,&ii[i].bdaddr,sizeof(devicename),devicename,0);
        memset(BLELOCAL, 0, sizeof(BLELOCAL));

        int Local_Name = hci_read_local_name(sock,8,BLELOCAL,0);
	int Feature = hci_le_read_remote_features(sock,handle,BLEFEATURE,0);


	if(read < 0){
	perror("Not able read:"); 
	exit(1);
	}
	if(Local_Name < 0){
	perror("Not able to read Local");
	exit(1);	
	}

	printf("Device name: %s\n local Name: %s\n Feature Name : %s\n", devicename,BLELOCAL,BLEFEATURE);
	
}

int i = 0;
while(i < num_rsp){

	int result = hci_create_connection(sock, &ii[i].bdaddr, ptype, clkoffset, rswitch, &handle, 10000);

        if(result < 0){
        perror("Connection Failed : ");
        exit(1);
        }
	i++;
}

    free(ii);
    close(sock);
    return 0;
}

