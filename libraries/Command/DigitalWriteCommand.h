//
//  Commander.h
//  
//
//  Created by Fabrizio on 10/12/14.
//
//

#ifndef DigitalWriteCommand_H
#define DigitalWriteCommand_H

#include "Command.h"

#define IDX_BUFFER_DIGITALWRITE_PIN 3
#define IDX_BUFFER_DIGITALWRITE_VALORE 4

class DigitalWriteCommand : public Command {
    
public:
    bool checkCommandRequested(char *buffer, char *bufTemp);
    int numberOfTokenRequested(char *buffer);
    int executeActionRequested(char *inStr, char *outStr);
    
    
};



#endif /* defined(DigitalWriteCommand_H) */
