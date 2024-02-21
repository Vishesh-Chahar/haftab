#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLEScan.h>

// The remote service we wish to connect to.

#define SERVICE_UUID        "cdf8bd1a-9d4c-11ee-8c90-0242ac120002"
#define CHARACTERISTIC_UUID "cdf8bf54-9d4c-11ee-8c90-0242ac120002"


//Pin connected to ST_CP of 74HC595
int latchPin = 34;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin[] = {23, 22, 21, 15, 18};



const byte numBytes = 5;
byte receivedBytes[numBytes];
byte numReceived = 0;

boolean newData = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  BLEDevice::init("haftab");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Ready to recieve words, but only 5 letters pls ;P");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x02);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  //set pins to output because they are addressed in the main loop

  pinMode(latchPin, OUTPUT);


  //Binary notation as comment
}



void loop() {

  char data_val = pCharacteristic=>getValue().c_str();
 
  delay(1000);

  recvBytesWithStartEndMarkers();

  for (int j = 0; j < 5; j++) {

    //load the light sequence you want from array

    data = recievedBytes[j];

    //ground latchPin and hold low for as long as you are transmitting

    digitalWrite(latchPin, 0);

    //move 'em out

    shiftOut(dataPin[j], clockPin, LSBFIRST, data_val);

    //return the latch pin high to signal chip that it

    //no longer needs to listen for information

    digitalWrite(latchPin, 1);

    delay(300);

  }
}
