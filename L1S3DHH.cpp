#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Wire.h>
#include <Adafruit_LIS3DH.h>

#define BLESERUUID "d618ad51-7aa1-49a7-891d-d05992a95d0c"
#define BLECHARUUID "0112-2334-4556-6778-899AABBCCDDEEFF0"

bool deviceConnected = false;
bool oldDeviceConnected = false;


#define BleServerName "PAVAN"

static BLEServer *pServer;
static BLERemoteCharacteristic *pCharacterAddress;
static BLECharacteristic *bCharacteristic = NULL;

Adafruit_LIS3DH lis = Adafruit_LIS3DH();

int num;
class MyServerCallbacks: public BLEServerCallbacks{
  void onConnect(BLEServer *pServer){
    deviceConnected = true;
  };
  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
  }
};

void setup() {
Serial.begin(115200);

  if (!lis.begin(0x18)) { // Change to 0x19 if your sensor has the address 0x19
    Serial.println("Couldnt start");
    while (1);
}

lis.setRange(LIS3DH_RANGE_4_G);
  
BLEDevice::init(BleServerName); //init BLE
pServer = BLEDevice::createServer();
pServer->setCallbacks(new MyServerCallbacks());

BLEService *bService = pServer->createService(BLESERUUID);
bCharacteristic = bService->createCharacteristic(BLECHARUUID,                     
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                 );
                                          
bCharacteristic->addDescriptor(new BLE2902());
bService->start();

BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
pAdvertising->addServiceUUID(BLESERUUID);
pAdvertising->setScanResponse(false);
pAdvertising->setMinPreferred(0x0);
//pAdvertising->setMinPreferred(0x12);
BLEDevice::startAdvertising();
}

void loop() {
char X_val,Y_val,Z_val;
lis.read();
String packets = "0x0201060303E1FF1216E1FFA10364";

if(deviceConnected){
//    Serial.println(num);
sprintf(X_val, "%.2f", lis.x);
sprintf(Y_val, "%.2f", lis.y);
sprintf(Z_val, "%.2f", lis.z);

String Value = String (packets) + String (X_val) + String  (Y_val) +String (Z_val);

  bCharacteristic->setValue(Value);
   // bCharacteristic->setValue(Y_val);
  bCharacteristic->notify(); 
   
  if(!deviceConnected && oldDeviceConnected) {
        delay(500);
        pServer->startAdvertising();
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
   if(deviceConnected && !oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
    }

//  Serial.println("Not Connected");

Serial.println("X : " + String(X_val));
Serial.println("Y : " + String(Y_val));
Serial.println("Z : " + String(Z_val));
delay(100);
}
     
