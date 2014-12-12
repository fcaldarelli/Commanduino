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
                while (client.available()) {
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
        if(packetSize>0)
        {
            Serial.print("Received packet of size ");
            Serial.println(packetSize);
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
            Serial.println("Contents:");
            Serial.print("Ricevuti bytes = ");
            Serial.println(nr);
            
            fncServerDispatchActions(packetBuffer, NULL);
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

void Commander::getBufferAtIndex(char *buffer, char *outBuffer, int index)
{
    int k = 0;
    char *p = buffer;
    char *pLast = buffer;
    bool trovato = false;
    
    outBuffer[0] = '\0';
    
    while((p!=NULL)&&(trovato == false))
    {
        pLast = p;
        
        if (buffer != pLast) pLast++;
        
        p = strstr(pLast, ";");
        
        if(p!=NULL)
        {
            if(index == k)
            {
                trovato = true;
                strncpy(outBuffer, pLast, p-pLast);
                outBuffer[p-pLast] = '\0';
                
            }
        }
        else
        {
            if (index == k)
            {
                trovato = true;
                strcpy(outBuffer, pLast);
            }
        }

        k++;
    }
}

int Commander::numberOfToken(char *buffer)
{
    int k = 0;
    int posStart = 0;
    char *p = buffer;
    
    while(p!=NULL)
    {
        if (buffer != p) p++;
        p = strstr(p, ";");
        k++;
    }
    
    return k;
}



