%% Clean workspace
clear; clc;

%% Experment Parameters
modeValue = 1;
totalRepsValue = 2;

%% Serial port 
arrayPort = 'COM4';
arrayBaud = 115200;
txPacketLen = 8;
rxPacketlen = 25;
writePacket = uint8(zeros(1, txPacketLen));
txHeader = uint8([155, 70, 69, 80]);
writePacket(1:4) = txHeader;
writePacket(5) = uint8(modeValue);
writePacket(6) = uint8(totalRepsValue);
checkSum = sum(writePacket);
writePacket(end-1) = uint8(floor(checkSum/256));
writePacket(end) = uint8(mod(checkSum,256));


%% Test array
arrayObj = serialport(arrayPort, arrayBaud, "Timeout", 5, "ByteOrder","little-endian");
pause(1);
write(arrayObj, writePacket,'uint8');
% while true
%     if arrayObj.NumBytesAvailable > rxPacketlen
%         data = read(arrayObj, rxPacketlen, "uint8");
%         data
%     end
% end
