/*
 * Serial Communication Packet class for transmitting button array state.
 * Code by Erick Nunez
 * Created 11/9/2021
 */

#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <Arduino.h>

class SerialPackets{
    public:
             SerialPackets(usb_serial_class * ptrSer);
        // bool CheckForRequest();
        bool DataAvailable();
        void ReadPackets();
        void SendStates(byte * targetArray, byte * buttonStates);
        void SendStart();
        void SendComplete();
        bool GetCompleteSent();
        byte GetRequestedMode();
        byte GetRequestedReps();
        bool GoodSetupRX();

    private:
        usb_serial_class * serialPort_M;
        const byte _REQUEST_HEADER[4]   = {155, 70, 69, 80};
        const byte _WRITE_HEADER[4]     = {175, 69, 80, 90};
        const int  _RX_PKT_LEN = 8;
        const int  _TX_PKT_LEN = 25;
        const int  _BAUDRATE = 115200;
        //bool dataRequested_M = false;
        bool completeSent_M = false;
        int16_t bytesToCounts(byte hByte, byte lByte);
        byte requestedMode_M = 0;
        byte requestedReps_M = 0;
        bool goodSetup_M = false;
};

#endif //SERIAL_COMM_H