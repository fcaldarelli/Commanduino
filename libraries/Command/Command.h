//
//  Commander.h
//  
//
//  Created by Fabrizio on 10/12/14.
//
//

#ifndef Command_H
#define Command_H

#include <Arduino.h>
#include "Commander.h"

#define IDX_BUFF_MACADDRESS 0
#define IDX_BUFF_MODE 1             // req, resp
#define IDX_BUFF_COMMAND 2          

#define CMD_BUFF_SEP ";"
#define CMD_BUFF_REQ "req"

class Command {
    
public:
    Command();
    ~Command();

public:
    int dispatch(char *inStr, char *outStr);
    void setCommander(Commander *cmdIn);

    
protected:
    void getBufferAtIndex(char *buffer, char *outBuffer, int index);
    int numberOfToken(char *buffer);
    bool checkBufferMacaddressWithLocal(char *buffer, char *tempBuff);
    bool isBufferMacaddressGeneric(char *buffer, char *tempBuff);
    void createResponseMessage(char *inStr, char *outStr);

    
protected:
    virtual bool checkCommandRequested(char *buffer, char *bufTemp) = 0;
    virtual int numberOfTokenRequested(char *buffer) = 0;
    virtual int executeActionRequested(char *buffer, char *outBuffer) = 0;
    
private:
    Commander *_commander;
    
    
    
};



#endif /* defined(Command_H) */
