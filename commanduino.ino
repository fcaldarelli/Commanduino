#include <Command.h>
#include <DigitalWriteCommand.h>

#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "Commander.h"
#include "DigitalWriteCommand.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x6D, 0xE4 };
byte ip[] = { 10, 9, 2, 111 };
byte dnsIP[] = { 0, 0, 0, 0 };
byte gateway[] = { 0, 0, 0, 0 };
byte subnet[] = { 0, 0, 0, 0 };

unsigned int localPort = 8888;      // local port to listen on

char packetBuffer[100]; //buffer to hold incoming packet,
char packetResponse[100];

EthernetUDP udp;
EthernetServer tcp(8888);
Commander commander;
DigitalWriteCommand digitalWriteCommand;

void (*fncDispatchActions)(char*, EthernetClient*);

void setup() {

  Serial.begin(9600);

  Ethernet.begin(mac, ip, dnsIP, gateway, subnet);

  fncDispatchActions = &dispatchActions;

  commander.setMacAddress(mac);
  commander.setTcp(&tcp);
  commander.tcpStartServer();
  commander.setUdp(&udp);
  commander.udpStartServer(localPort);
  
  // Set commander for commands
   digitalWriteCommand.setCommander(&commander);
  
}

void dispatchActions(char *pktBuffer, EthernetClient *client)
{
    if( digitalWriteCommand.dispatch(pktBuffer, packetResponse) == 0) commander.sendResponse(packetResponse, client);
}


void loop() {

  commander.udpDispatch(packetBuffer, 100, fncDispatchActions);
  commander.tcpDispatch(packetBuffer, 100, fncDispatchActions);

  /*
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int nr = Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.print("Ricevuti bytes = ");
    Serial.println(nr);


    // send a reply, to the IP address and port that sent us the packet we received
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write("ciao");
    Udp.endPacket();
    */
  /*
      Udp.stop();
        Ethernet.begin(mac,ip);
        Udp.begin(localPort);
  */

  delay(100);
}
