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

To build the connection with Accelometer assign some value 

        bdaddr_t peer_bdaddr;
    str2ba(:Accelometer mac address", &peer_bdaddr);

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

FIND Scan the Mac OF BLE Device

	ba2str(&ii[i].bdaddr,BLEADD);

STORE THE NAME OF DEVICE IN BUFFER

	int read = hci_read_remote_name(sock,&ii[i].bdaddr,sizeof(devicename),devicename,0);

 To BUILD cONNECTION This API WILL HELP

     int r = hci_le_create_conn(sock, interval, window, initiator_filter, peer_bdaddr_type,
        peer_bdaddr, own_bdaddr_type, min_interval, max_interval, latency,
        supervision_timeout, min_ce_length, max_ce_length, &handle, CONNECT_TIMEOUT * 1000000);
