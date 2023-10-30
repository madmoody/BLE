#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/bluez/attrib/gatt.h>

char *MAC_ACCADD  = "12:34:56:78:90:00";
char *characteristic_uuid = "0x0112233445566778899AABBCCDDEEFFO";

int main() {
    int BLE_id = hci_get_route(NULL);
    int sock = hci_open_dev(BLE_id);
    if (BLE_id < 0) {
        perror("HCI device open failed");
        exit(1);
    }

    
    inquiry_info* ii = NULL;
    int max_rsp = 255;
    int num_rsp = 0;
    long flags = IREQ_CACHE_FLUSH;
    num_rsp = hci_inquiry(BLE_id, 8, max_rsp, NULL, &ii, flags);
    if (num_rsp < 0) {
        perror("HCI inquiry failed");
        close(sock);
        exit(1);
    }


char BLEADD[100];

while(1){
	for(int i = 0;i<num_rsp;i++){
	ba2str(&ii[i].bdaddr,BLEADD);

	if(strcmp(BLEADD,MAC_ACCADD) == 0){
	printf("Device found %s\n:",BLEADD);

	 GattAttribute* attr = NULL;
    while (1) {
        attr = find_characteristic_by_uuid(characteristic_uuid);
        if (attr != NULL) {
            break;
        }
        sleep(1);
    }
        length = gatt_read(sock, attr, buffer, sizeof(buffer));

        if(length < 0) {
        perror("Failed to read characteristic value");
        exit(1);
        }


        for (int i = 0; i < length; i++) {
        printf("%02X ", buffer[i]);

        printf("\n");
        }


}
else{
	printf("Waiting..\n");
}}}
    free(ii);


    
    close(sock);
    return 0;
}


