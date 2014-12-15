//
//  Commander.h
//  
//
//  Created by Fabrizio on 10/12/14.
//
//

#ifndef DigitalReadCommand_H
#define DigitalReadCommand_H

#include "Command.h"

#define IDX_BUFFER_DIGITALREAD_PIN 3

class DigitalReadCommand : public Command {
    
public:
    bool checkCommandRequested(char *buffer, char *bufTemp);
    int numberOfTokenRequested(char *buffer);
    int executeActionRequested(char *inStr, char *outStr);
    
    
};



#endif /* defined(DigitalReadCommand_H) */
