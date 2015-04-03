Commanduino
===========

This software helps you to command Arduino boards through network, using udp or tcp protocol.

The software is built on two main classes: Commander and Command.

Infact main code file commanduino.ino is only for initializations and calls to Commander's dispatcher functions.

Commander class
===============

Commander handles network initialization and communication, udp and tcp.

    commander.setMacAddress(mac);
    commander.setTcp(&tcp);
    commander.tcpStartServer();
    commander.setUdp(&udp);
    commander.udpStartServer(localPort);

In commanduino.ino we use both udp and tcp protocols. Commander starts to listen to `localPort` (default 8888) tcp and udp incoming connections.

At last in `loop()` method, dispatch from network, passing data to pointer function `fncDispatchActions`.


Command class
=============

Command is the base class for all events management. Now, at time of writing, there is only a subclass, DigitalWriteCommand, that implements requests to change digital status of pin.

Infact DigitaWriteCommand implements the three abstract methods: 
- numberOfTokenRequested: when parse incoming data, we check that number of token (words separated by semicolons) is correct for this command handler;
- checkCommandRequested: when parse incoming data, check fields consistency; in case of DigitalWriteCommand, verify that second field is req (that is request) and the third field is ´digwr´ (that is digital write command); so we are sure that command request is exactly this;
- executeActionRequested: called when numberOfTokenRequested and checkCommandRequested matches, there we execute really the request (in the case of DigitalWriteCommand to change status of digital pin);

In main code, after Commander class had dispatched new incoming connection, it will be called function pointer `fncDispatchActions`. In body of this function we find all Command subclasses to be use to check requested data.


How implement new actions
=========================

It is very simple to implement new action (analog write, analog read, digital write, digital read,...) and I'll do this as soon as possible.

You have to subclass Command class and override previous three methods.


How to use
==========

Use PacketSender to make tests. This is an opensource software available for all platforms (Windows, Linux, Mac), downloadable from http://packetsender.com/

Follow these instructions:

1. Open commanduino.ino and change mac (row 12), ip (row 13) if you need tcp connection and local port (row 18), if 8888 is wrong.
2. Save and load commanduino.ino into your Arduino board.
3. Send a packet using specific format (using, for example, 255.255.255.255 as destination address, 8888 as port to connect and udp for protocol);


![](https://cloud.githubusercontent.com/assets/4108673/5416418/7bc60cf6-822f-11e4-883a-9da7927827bc.jpg)


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


