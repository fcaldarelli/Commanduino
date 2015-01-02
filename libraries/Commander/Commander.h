//
//  Commander.h
//  
//
//  Created by Fabrizio on 10/12/14.
//
//

#ifndef Commander_H
#define Commander_H

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

class Commander {
    
public:
    Commander();
    ~Commander();
    
    
    
public:
    byte* getMacAddress();
    void setMacAddress(byte *btMac);
    void setUdp(EthernetUDP *udpIn);
    void setTcp(EthernetServer *tcpIn);

    int tcpStartServer();
    int tcpDispatch(char *packetBuffer, int maxLengthPacketBuffer, void (*fncServerDispatchActions)(char*, EthernetClient*));
    void tcpSend(char *buffer, EthernetClient* client);

    int udpStartServer(int localPort);
    int udpDispatch(char *packetBuffer, int maxLengthPacketBuffer, void (*fncServerDispatchActions)(char*, EthernetClient*));
    void udpSend(char *buffer);
    
    void sendResponse(char *buffer, EthernetClient* client);

    
private:
    EthernetUDP *_udp;
    EthernetServer *_tcp;
    byte *_macAddress;
    
};



#endif /* defined(Commander_H) */
