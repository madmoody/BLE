# BLE CLient

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
    str2ba("12:34:56:78:90:00", &peer_bdaddr);

    uint16_t handle = 0;
    uint8_t initiator_filter = 0;
    uint8_t peer_bdaddr_type = LE_PUBLIC_ADDRESS;
    uint8_t own_bdaddr_type = LE_PUBLIC_ADDRESS;
    uint16_t ptype = 0x0800;   
    uint16_t clkoffset = 0x0000; 
    uint8_t rswitch = 0x01;  

CHECK THE CONNECTION

    num_rsp = hci_inquiry(BLE_id, 8, max_rsp, NULL, &ii, flags);
    if (num_rsp < 0) {
        perror("HCI inquiry failed");
        close(sock);
        exit(1);
    }

FIND Scan the Mac OF BLE Device

	ba2str(&ii[i].bdaddr,BLEADD);
 	 	 
STORE THE NAME OF DEVICE IN BUFFER

	int read = hci_read_remote_name(sock,&ii[i].bdaddr,sizeof(devicename),devicename,0);

 STORE LOCAL NAME OF DEVICE 

  	int Local_Name = hci_read_local_name(sock,8,BLELOCAL,0);
	
 To BUILD cONNECTION This API WILL HELP

     	int result = hci_create_connection(sock, &ii[i].bdaddr, ptype, clkoffset, rswitch, &handle, 10000);


# BLE SERVER

Take ESP32 Microcontroller Along with connect LIS3DH Accelerometer 

Install Arduino IDE Install Libraries in Library manager

	#include <BLEUtils.h>
	#include <BLEServer.h>
	#include <BLE2902.h>
	#include <Wire.h>
	#include <Adafruit_LIS3DH.h>

Generate UUID for Server and Characteristics
	#define BLESERUUID "d618ad51-7aa1-49a7-891d-d05992a95d0c"
	#define BLECHARUUID "0112-2334-4556-6778-899AABBCCDDEEFF0"

Give Server name and characteristic name 

	static BLEServer *pServer;
	static BLERemoteCharacteristic *pCharacterAddress;
	static BLECharacteristic *bCharacteristic = NULL;
	Adafruit_LIS3DH lis = Adafruit_LIS3DH()

Start accelerometer with 0x18 address of device

 	if (!lis.begin(0x18)) {
        Serial.println("Couldnt start");
        while (1);}

Create Server and characteristic uuid

	BLEDevice::init(BleServerName);
	pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());
	BLEService *bService = pServer->createService(BLESERUUID);
	bCharacteristic = bService->createCharacteristic(BLECHARUUID,                     
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                 );

Start Advertising The device

 	bService->start();
	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(BLESERUUID);
	pAdvertising->setScanResponse(false);
	pAdvertising->setMinPreferred(0x0);
	//pAdvertising->setMinPreferred(0x12);
	BLEDevice::startAdvertising();

Value will Notify if anychange

	bCharacteristic->setValue(Value);
   	bCharacteristic->notify(); 
