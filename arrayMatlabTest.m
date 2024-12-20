%% Clean workspace
clear; clc;

%% Experment Parameters
modeValue = 2;
totalRepsValue = 3;

%% Serial port 
arrayPort = 'COM4';
arrayBaud = 115200;
txPacketLen = 8;
rxPacketlen = 18;
writePacket = uint8(zeros(1, txPacketLen));
txHeader = uint8([155, 70, 69, 80]);
writePacket(1:4) = txHeader;
writePacket(5) = uint8(modeValue);
writePacket(6) = uint8(totalRepsValue);
checkSum = sum(writePacket);
writePacket(end-1) = uint8(floor(checkSum/256));
writePacket(end) = uint8(mod(checkSum,256));


%% Test array
arrayObj = serialport(arrayPort, arrayBaud, "Timeout", 5);
pause(1);
write(arrayObj, writePacket,'uint8');
running = true;
while running
    if arrayObj.NumBytesAvailable >= rxPacketlen
        data = read(arrayObj, rxPacketlen, "uint8");
        data
        if data(6) == uint8(200)
            break
        end
    end
end

%% Clean up
clear arrayObj
