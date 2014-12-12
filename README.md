Commanduino
===========

This software helps you to command Arduino boards through network, using udp or tcp protocol

Follow these instructions:

1. Open commanduino.ino and change mac (row 12), ip (row 13) if you need tcp connection and local port (row 18), if 8888 is wrong.
2. Save and load commanduino.ino into your Arduino board.
3. Send a packet using specific format (using, for example, 255.255.255.255 as destination address, 8888 as port to connect and udp for protocol);





Packet Format
=============

[macaddress:12];[resp|req];[command];[values;;;;;]

- macaddress: this field is macaddress destination of message; if you send broadcast message
- req|resp: this field could be a request "req" or response "resp"; if you send a command that needs response (for example analog input level), first message to read input value is a "req" and response message contains "resp";
- command: command to execute (es. digwr for digital write);
- values: list of values commas separated; for digital write, values are two: pin and value.

An example of command to digital write value 1 on pin 9 is:

`90A2DA0D6DE4;req;digwr;9;1`

if destination arduino has macaddress `90A2DA0D6DE4`

that will answer with:

`90A2DA0D6DE4;resp;digwr;9;1`


