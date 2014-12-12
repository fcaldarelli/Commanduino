//
//  Commander.cpp
//  
//
//  Created by Fabrizio on 10/12/14.
//
//

#include "DigitalWriteCommand.h"

int DigitalWriteCommand::numberOfTokenRequested(char *buffer)
{
    return 5;
}

bool DigitalWriteCommand::checkCommandRequested(char *buffer, char *bufTemp)
{
    bool retVal = false;
    
    this->getBufferAtIndex(buffer, bufTemp, IDX_BUFF_MODE);
    if(strcmp(bufTemp, "req")==0)
    {
        this->getBufferAtIndex(buffer, bufTemp, IDX_BUFF_COMMAND);
        if(strcmp(bufTemp, "digwr")==0)
        {
            retVal = true;
        }
    }
    
    return retVal;
}


int DigitalWriteCommand::executeActionRequested(char *inBuf, char *outBuf)
{
    byte iPin;
    byte iValore;
    
    this->getBufferAtIndex(inBuf, outBuf, IDX_BUFFER_DIGITALWRITE_PIN);
    iPin = atoi(outBuf);
    this->getBufferAtIndex(inBuf, outBuf, IDX_BUFFER_DIGITALWRITE_VALORE);
    iValore = atoi(outBuf);
    
    // Set Pin Value
    pinMode(iPin, OUTPUT);
    digitalWrite(iPin, iValore);

    this->createResponseMessage(inBuf, outBuf);
    
    return 0;
}



