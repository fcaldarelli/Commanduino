//
//  Commander.cpp
//  
//
//  Created by Fabrizio on 10/12/14.
//
//

#include "Commander.h"
#include "Arduino.h"

// Destructor
Commander::~Commander()
{

}

// Constructor
Commander::Commander()
{
    
}

byte* Commander::getMacAddress()
{
    return this->_macAddress;
}
void Commander::setMacAddress(byte *btMac)
{
    this->_macAddress = btMac;
}
void Commander::setUdp(EthernetUDP *udpIn)
{
    this->_udp = udpIn;
}
void Commander::setTcp(EthernetServer *tcpIn)
{
    this->_tcp = tcpIn;
}

int Commander::tcpStartServer()
{
    if(_tcp != NULL)
    {
        _tcp->begin();
        return 0;
    }
    else
    {
        return -1;
    }
}

int Commander::tcpDispatch(char *packetBuffer, int maxLengthPacketBuffer, void (*fncTcpServerDispatchActions)(char*, EthernetClient*))
{
    int packetSize = 0;
    
    if (_tcp != NULL)
    {
        EthernetClient client = _tcp->available();
        
        if(client)
        {
            Serial.println("TCP DISPONIBILE");
            packetBuffer[0] = '\0';
        
            while (client.connected()) {
                while ((client.available())&&(packetSize<maxLengthPacketBuffer)) {
                    char c = client.read();
                    if (c!=0)
                    {
                        packetBuffer[packetSize] = c;
                        packetBuffer[packetSize+1] = 0;
                        packetSize++;
                    }
                }
                
                fncTcpServerDispatchActions(packetBuffer, &client);
                
                client.stop();
            }
            Serial.println("TCP SCONNESSO");
            
        }
        
    }
    return packetSize;
}

void Commander::tcpSend(char *buffer, EthernetClient* client)
{
    client->write(buffer);
}

void Commander::udpSend(char *buffer)
{
    if(this->_udp != NULL)
    {
        this->_udp->beginPacket(this->_udp->remoteIP(), this->_udp->remotePort());
        this->_udp->write(buffer);
        this->_udp->endPacket();
    }
}



int Commander::udpStartServer(int localPort)
{
    if(_udp != NULL)
    {
        _udp->begin(localPort);
        return 0;
    }
    else
    {
        return -1;
    }
}


int Commander::udpDispatch(char *packetBuffer, int maxLengthPacketBuffer, void (*fncServerDispatchActions)(char*, EthernetClient*))
{
    int packetSize = 0;
    
    if (_udp != NULL)
    {
        packetSize = _udp->parsePacket();
        
        Serial.print("Received packet of size ");
        Serial.println(packetSize);
        
        if((packetSize>0)&&(packetSize<maxLengthPacketBuffer))
        {
            Serial.print("From ");
            IPAddress remote = _udp->remoteIP();
            for (int i =0; i < 4; i++)
            {
                Serial.print(remote[i], DEC);
                if (i < 3)
                {
                    Serial.print(".");
                }
            }
            Serial.print(", port ");
            Serial.println(_udp->remotePort());
            
            // read the packet into packetBufffer
            int nr = _udp->read(packetBuffer,maxLengthPacketBuffer);
            /*
            if(nr>maxLengthPacketBuffer)
            {
                packetBuffer[maxLengthPacketBuffer] = '\0';
            }
            else
            {
                packetBuffer[nr] = '\0';
            }
            */
            
            Serial.print("Ricevuti bytes = ");
            Serial.println(nr);
            Serial.print("data = ");
            Serial.println(packetBuffer);
            
            
            fncServerDispatchActions(packetBuffer, NULL);
        }
        else
        {
            Serial.println("Packet too long");
        }
    }
    return packetSize;
}

void Commander::sendResponse(char *buffer, EthernetClient* client)
{
        if(client!=NULL)
        {
            tcpSend(buffer, client);
        }
        else
        {
            udpSend(buffer);
        }
    
}




