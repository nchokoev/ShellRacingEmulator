#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID_FFF        "0000fff0-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_FFF1 "d44bc439-abfd-45a2-b575-925416129600" /*- Write, Write no response */
#define CHARACTERISTIC_UUID_FFF2 "d44bc439-abfd-45a2-b575-92541612960a" /*- Write, Write no response */
#define CHARACTERISTIC_UUID_FFF3 "d44bc439-abfd-45a2-b575-92541612960b" /*- Write, Write no response */
#define CHARACTERISTIC_UUID_FFF4 "d44bc439-abfd-45a2-b575-925416129601" /*- Notify */

#define SERVICE_UUID_FD        "0000fd00-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_FD1 "0000fd01-0000-1000-8000-00805f9b34fb" /* - Write no response */
#define CHARACTERISTIC_UUID_FD2 "0000fd02-0000-1000-8000-00805f9b34fb" /* - Notify, Write */

uint8_t idle_data[16] = {0x02, 0x5e, 0x69, 0x5a, 0x48, 0xff, 0x2a, 0x43, 0x8c, 0xa6, 0x80, 0xf8, 0x3e, 0x04, 0xe4, 0x5d};
uint8_t up_data[16] = {0x29, 0x60, 0x9c, 0x66, 0x48, 0x52, 0xcf, 0xf1, 0xb0, 0xf0, 0xcb, 0xb9, 0x80, 0x14, 0xbd, 0x2c};
uint8_t down_data[16] = {0x03, 0x20, 0x99, 0x09, 0xba, 0x9d, 0xa1, 0xc8, 0xb9, 0x86, 0x16, 0x3c, 0x6d, 0x48, 0x46, 0x55};
uint8_t left_data[16] = {0x51, 0x38, 0x21, 0x12, 0x13, 0x5c, 0xcc, 0xdb, 0x46, 0xcf, 0x89, 0x21, 0xb7, 0x05, 0x49, 0x9a};
uint8_t right_data[16] = {0x1b, 0x57, 0x69, 0xcd, 0xf1, 0x3e, 0x8a, 0xb6, 0x27, 0x08, 0x0f, 0xf3, 0xce, 0xfc, 0x3b, 0xc0};

uint8_t up_left_data[16] = {0x99, 0x28, 0xe5, 0x90, 0xdf, 0xe8, 0x21, 0x48, 0x5f, 0x41, 0x4f, 0xbb, 0x63, 0x3d, 0x5c, 0x4e};
uint8_t up_right_data[16] = {0x0f, 0x2c, 0xe5, 0x66, 0x62, 0xd4, 0xfd, 0x9d, 0x32, 0xa4, 0x4f, 0x10, 0x2b, 0xf2, 0x0a, 0xa7};
uint8_t down_left_data[16] = {0x98, 0xce, 0x98, 0x1d, 0x58, 0xd1, 0x15, 0xaf, 0xe1, 0x19, 0x60, 0xbf, 0x46, 0x13, 0x92, 0x5c};
uint8_t down_right_data[16] = {0xf2, 0x52, 0x0f, 0xba, 0x31, 0x44, 0xfb, 0x11, 0x46, 0x8f, 0xe0, 0x80, 0xc6, 0xc2, 0xc2, 0x3c};

bool compare_ar(uint8_t* a1, uint8_t* a2, int sz)
{
    for(int i=0; i < sz; i++)
    {
        if(a1[i] != a2[i])
        {
            return false;
        }
    }
    return true;
}

void PrintHex8(uint8_t *data, uint8_t length) // prints 8-bit data in hex
{
     char tmp[length*2+1];
     byte first;
     byte second;
     for (int i=0; i<length; i++) {
           first = (data[i] >> 4) & 0x0f;
           second = data[i] & 0x0f;
           // base for converting single digit numbers to ASCII is 48
           // base for 10-16 to become lower-case characters a-f is 87
           // note: difference is 39
           tmp[i*2] = first+48;
           tmp[i*2+1] = second+48;
           if (first > 9) tmp[i*2] += 39;
           if (second > 9) tmp[i*2+1] += 39;
     }
     tmp[length*2] = 0;
     Serial.println(tmp);
}

class fff1_callbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        uint8_t* data = pCharacteristic->getData();

        if (value.length() > 0)
        {
            if(value.length() != 16)
            {
                Serial.print("FFF1 [");
                Serial.print(value.length(), DEC);
                Serial.print("] value: ");
                PrintHex8(data, value.length());
                // for (int i = 0; i < value.length(); i++)
                // Serial.print(data[i], HEX);
                //Serial.println();
            }
            else if(compare_ar(data, idle_data, 16) == true)
            {
                //Serial.println("");
            }
            else if(compare_ar(data, up_data, 16) == true)
            {
                Serial.println("UP");
            }
            else if(compare_ar(data, down_data, 16) == true)
            {
                Serial.println("DOWN");
            }
            else if(compare_ar(data, left_data, 16) == true)
            {
                Serial.println("LEFT");
            }
            else if(compare_ar(data, right_data, 16) == true)
            {
                Serial.println("RIGHT");
            }
            else if(compare_ar(data, up_left_data, 16) == true)
            {
                Serial.println("UP_LEFT");
            }
            else if(compare_ar(data, up_right_data, 16) == true)
            {
                Serial.println("UP_RIGHT");
            }
            else if(compare_ar(data, down_left_data, 16) == true)
            {
                Serial.println("DOWN_LEFT");
            }
            else if(compare_ar(data, down_right_data, 16) == true)
            {
                Serial.println("DOWN_RIGHT");
            }
            else
            {
                Serial.print("FFF1 [");
                Serial.print(value.length(), DEC);
                Serial.print("] value: ");
                PrintHex8(data, value.length());
            }
            
        }
    }
};
class fff2_callbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        uint8_t* data = pCharacteristic->getData();

        if (value.length() > 0) {
            Serial.print("FFF2 value: ");
            for (int i = 0; i < value.length(); i++)
            Serial.print(data[i], HEX);
            Serial.println();
        }
    }
};
class fff3_callbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        uint8_t* data = pCharacteristic->getData();

        if (value.length() > 0) {
            Serial.print("FFF3 value: ");
            for (int i = 0; i < value.length(); i++)
            Serial.print(data[i], HEX);
            Serial.println();
        }
    }
};
class fff4_callbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        uint8_t* data = pCharacteristic->getData();

        if (value.length() > 0) {
            Serial.print("FFF4 value: ");
            for (int i = 0; i < value.length(); i++)
            Serial.print(data[i], HEX);
            Serial.println();
        }
    }
};

class fd1_callbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        uint8_t* data = pCharacteristic->getData();

        if (value.length() > 0) {
            Serial.print("FD1 value: ");
            for (int i = 0; i < value.length(); i++)
            Serial.print(data[i], HEX);
            Serial.println();
        }
    }
};

class fd2_callbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
        std::string value = pCharacteristic->getValue();
        uint8_t* data = pCharacteristic->getData();

        if (value.length() > 0) {
            Serial.print("FD2 value: ");
            for (int i = 0; i < value.length(); i++)
            Serial.print(data[i], HEX);
            Serial.println();
        }
    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

//    BLEDevice::init("QCAR-01F371");
    BLEDevice::init("QCAR-01F373");
    BLEServer *pServer = BLEDevice::createServer();
    BLEService *pService_fff = pServer->createService(SERVICE_UUID_FFF);
    BLECharacteristic *pCharacteristic_fff1 = pService_fff->createCharacteristic(CHARACTERISTIC_UUID_FFF1,
                                            BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR );
    BLECharacteristic *pCharacteristic_fff2 = pService_fff->createCharacteristic(CHARACTERISTIC_UUID_FFF2,
                                            BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR );
    BLECharacteristic *pCharacteristic_fff3 = pService_fff->createCharacteristic(CHARACTERISTIC_UUID_FFF3,
                                            BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR );
    BLECharacteristic *pCharacteristic_fff4 = pService_fff->createCharacteristic(CHARACTERISTIC_UUID_FFF4,
                                            BLECharacteristic::PROPERTY_INDICATE );


    pCharacteristic_fff1->setCallbacks(new fff1_callbacks());
    pCharacteristic_fff2->setCallbacks(new fff2_callbacks());
    pCharacteristic_fff3->setCallbacks(new fff3_callbacks());
    pCharacteristic_fff4->setCallbacks(new fff4_callbacks());

    BLEService *pService_fd = pServer->createService(SERVICE_UUID_FD);
    BLECharacteristic *pCharacteristic_fd1 = pService_fd->createCharacteristic(CHARACTERISTIC_UUID_FD1,
                                            BLECharacteristic::PROPERTY_WRITE_NR );
    BLECharacteristic *pCharacteristic_fd2 = pService_fd->createCharacteristic(CHARACTERISTIC_UUID_FD2,
                                            BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE );
    pCharacteristic_fd1->setCallbacks(new fd1_callbacks());
    pCharacteristic_fd2->setCallbacks(new fd2_callbacks());

    pService_fff->start();
    pService_fd->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID_FFF);
    pAdvertising->addServiceUUID(SERVICE_UUID_FD);

    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
    // put your main code here, to run repeatedly:
    delay(2000);
}