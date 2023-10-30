# BLE

Need to install bluez development package for linux/macos so where we can connect with macadress

    sudo apt-get install libbluetooth-dev

THESE ARE THE BLE LIBRARY MENTION ALONG WITH PATH

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <bluetooth/bluetooth.h>
    #include <bluetooth/hci.h>
    #include <bluetooth/hci_lib.h>
    #include <bluetooth/bluez/attrib/gatt.h>


MAC ADRESS OF ACCELOMETER AND CHARACTERISTIC ID 

    char *MAC_ACCADD  = "12:34:56:78:90:00";
    char *characteristic_uuid = "0x0112233445566778899AABBCCDDEEFFO";

INITIALISE THE BLE IN LINUX

    int BLE_id = hci_get_route(NULL);
    int sock = hci_open_dev(BLE_id);
    if (BLE_id < 0) {
        perror("HCI device open failed");
        exit(1);
        }

SCAN FOR THE ACCELEROMETER

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

CHECK THE MAC ADRESS OF THE ACCELEOMETER AND PRINT MAC ADDRES

        if(strcmp(BLEADD,MAC_ACCADD) == 0){
        printf("Device found %s\n:",BLEADD);

FIND THE CHARACTERISTIC UUID OF ACCELEROMETER

        attr = find_characteristic_by_uuid(characteristic_uuid);

STORE THE VALUE IN BUFFER

        length = gatt_read(sock, attr, buffer, sizeof(buffer));
