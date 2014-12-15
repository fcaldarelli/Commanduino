//
//  Commander.cpp
//  
//
//  Created by Fabrizio on 10/12/14.
//
//

#include "DigitalReadCommand.h"

int DigitalReadCommand::numberOfTokenRequested(char *buffer)
{
    return 4;
}

bool DigitalReadCommand::checkCommandRequested(char *buffer, char *bufTemp)
{
    bool retVal = false;
    
    this->getBufferAtIndex(buffer, bufTemp, IDX_BUFF_MODE);
    if(strcmp(bufTemp, CMD_BUFF_REQ)==0)
    {
        this->getBufferAtIndex(buffer, bufTemp, IDX_BUFF_COMMAND);
        if(strcmp(bufTemp, "digrd")==0)
        {
            retVal = true;
        }
    }
    
    return retVal;
}


int DigitalReadCommand::executeActionRequested(char *inBuf, char *outBuf)
{
    byte iPin;
    byte iValore;
    
    this->getBufferAtIndex(inBuf, outBuf, IDX_BUFFER_DIGITALREAD_PIN);
    iPin = atoi(outBuf);
    
    // Set Pin Value
    pinMode(iPin, OUTPUT);
    iValore = digitalRead(iPin);
    
    this->createResponseMessage(inBuf, outBuf);
    
    char bufTemp[3] = "";
    itoa(iValore, bufTemp, 10);
    
    strcat(outBuf, CMD_BUFF_SEP);
    strcat(outBuf, bufTemp);
    
    return 0;
}



