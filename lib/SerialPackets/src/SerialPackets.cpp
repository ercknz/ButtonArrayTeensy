#include "SerialPackets.h"
#include <Arduino.h>

SerialPackets::SerialPackets(usb_serial_class * ptrSer){
    serialPort_M = ptrSer;
}

bool SerialPackets::DataAvailable(){
    return serialPort_M->available();
}

byte SerialPackets::GetRequestedMode(){
    return requestedMode_M;
}

byte SerialPackets::GetRequestedReps(){
    return requestedReps_M;
}

bool SerialPackets::GoodSetupRX(){
    return goodSetup_M;
}

void SerialPackets::ReadPackets(){
    byte RXPacket[_RX_PKT_LEN];
    byte tempHeader[4];
    int16_t SumCheck;
    int16_t CHECKSUM;
    unsigned long timeOutTime = millis();
    while (serialPort_M->available() < _RX_PKT_LEN){
        if (millis() - timeOutTime > 5){
            while(serialPort_M->available()) serialPort_M->read();
            return;
        }
    }
    for (int i = 0; i < _RX_PKT_LEN; i++){
        RXPacket[i] = serialPort_M -> read();
    }

    CHECKSUM = bytesToCounts(RXPacket[_RX_PKT_LEN - 2], RXPacket[_RX_PKT_LEN - 1]);
    SumCheck = 0;
    for ( int i = 0; i < _RX_PKT_LEN - 2; i++){
        SumCheck += RXPacket[i];
    }
    if (SumCheck != CHECKSUM) return;

    for (int i = 0; i < 4; i++){
        tempHeader[i] = RXPacket[i];
    }
    if (memcmp(_REQUEST_HEADER, tempHeader, sizeof(_REQUEST_HEADER)) == 0){
        requestedMode_M = RXPacket[4];
        requestedReps_M = RXPacket[5];
        goodSetup_M = true;
        return;
    }
}

void SerialPackets::SendStates(byte * targetArray, byte * buttonStates){
    byte dataPacket[_TX_PKT_LEN] = {0};
    uint16_t packetSum = 0;

    // Header [175 69 80 90]
    for (int i = 0; i < 4; i++){
        dataPacket[i] = _WRITE_HEADER[i];
    }
    // Target [T1 T2 T3 T4 T5 T6 T7 T8 T9]
    for (int i = 0; i < 9; i++){
        dataPacket[i + 4] = targetArray[i];
    }
    // Button States [B0 B1 B2 B3 B4 B5 S1 S2 S3 S4]
    for (int i = 0; i < 10; i++){
        dataPacket[i + 13] = !buttonStates[i];
    }
    // Check Sum
    for (int i = 0; i < _TX_PKT_LEN - 2; i++){
        packetSum += dataPacket[i];
    }
    dataPacket[_TX_PKT_LEN - 2] = floor(packetSum / 256);
    dataPacket[_TX_PKT_LEN - 1] = floor(packetSum % 256);

    // Packet Write
    for (int i = 0; i < _TX_PKT_LEN; i++){
        serialPort_M -> write(dataPacket[i]);
    }
}

void SerialPackets::SendStart(){
    byte dataPacket[_TX_PKT_LEN] = {0};
    uint16_t packetSum = 0;
    for (int i = 0; i < 4; i++){
        dataPacket[i] = _WRITE_HEADER[i];
    }
    dataPacket[5] = 100; 
    for (int i = 0; i < _TX_PKT_LEN - 2; i++){
        packetSum += dataPacket[i];
    }
    dataPacket[_TX_PKT_LEN - 2] = floor(packetSum / 256);
    dataPacket[_TX_PKT_LEN - 1] = floor(packetSum % 256);
    for (int i = 0; i < _TX_PKT_LEN; i++){
        serialPort_M -> write(dataPacket[i]);
    } 
}

void SerialPackets::SendComplete(){
    byte dataPacket[_TX_PKT_LEN] = {0};
    uint16_t packetSum = 0;
    for (int i = 0; i < 4; i++){
        dataPacket[i] = _WRITE_HEADER[i];
    }
    dataPacket[5] = 200; 
    for (int i = 0; i < _TX_PKT_LEN - 2; i++){
        packetSum += dataPacket[i];
    }
    dataPacket[_TX_PKT_LEN - 2] = floor(packetSum / 256);
    dataPacket[_TX_PKT_LEN - 1] = floor(packetSum % 256);
    for (int i = 0; i < _TX_PKT_LEN; i++){
        serialPort_M -> write(dataPacket[i]);
    }
    completeSent_M = true;
}
        
bool SerialPackets::GetCompleteSent(){
    return completeSent_M;
}

int16_t SerialPackets::bytesToCounts(byte hByte, byte lByte) {
  int16_t decimal = hByte * 256 + lByte;
  return decimal;
}