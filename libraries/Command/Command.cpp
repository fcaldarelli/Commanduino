//
//  Commander.cpp
//  
//
//  Created by Fabrizio on 10/12/14.
//
//

#include "Command.h"

Command::Command()
{

}

Command::~Command()
{
    
}

void Command::setCommander(Commander *cmdIn)
{
    this->_commander = cmdIn;
}

bool Command::checkBufferMacaddressWithLocal(char *buffer, char *tempBuff)
{
    byte *btMac = NULL;
    
    bool retVal = false;
    
    if(this->_commander!=NULL)
    {
        btMac = this->_commander->getMacAddress();
        if(btMac != NULL)
        {
            
            this->getBufferAtIndex(buffer, tempBuff, IDX_BUFF_MACADDRESS);
            
            if(strlen(tempBuff) == 12)
            {
                char strMac[12];
                for(byte k=0;k<6;k++)
                {
                    sprintf(strMac+(k*2), "%02X", btMac[k]);
                }
                if(strcmp(strMac, tempBuff) == 0)
                {
                    retVal = true;
                }
            }
            
        }
    }
    
    return retVal;
}

bool Command::isBufferMacaddressGeneric(char *buffer, char *tempBuff)
{
    bool retVal = false;
    
    if(this->_commander!=NULL)
    {
        this->getBufferAtIndex(buffer, tempBuff, IDX_BUFF_MACADDRESS);
        
        if(strlen(tempBuff) == 12)
        {
            if(strcmp("000000000000", tempBuff) == 0)
            {
                retVal = true;
            }
        }
    }
    
    return retVal;
}


void Command::getBufferAtIndex(char *buffer, char *outBuffer, int index)
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

int Command::numberOfToken(char *buffer)
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

void Command::createResponseMessage(char *inStr, char *outStr)
{
    byte *btMac = NULL;
    
    outStr[0] = '\0';
    
    if(this->_commander!=NULL)
    {
        btMac = this->_commander->getMacAddress();
        if(btMac != NULL)
        {
            for(byte k=0;k<6;k++)
            {
                sprintf(outStr+(k*2), "%02X", btMac[k]);
            }
            strcpy(outStr+12, ";resp");
            strcat(outStr+17, strstr(inStr+13, ";"));
        }
    }
}

int Command::dispatch(char *inStr, char *outStr)
{
    int retVal = 0;
    
    if(this->numberOfToken(inStr) != this->numberOfTokenRequested(inStr))
    {
        retVal = -1;
    }
    else
    {
        if(this->checkCommandRequested(inStr, outStr))
        {
            if(
                (this->checkBufferMacaddressWithLocal(inStr, outStr))
               ||
               (this->isBufferMacaddressGeneric(inStr, outStr))
            )
            {
                retVal = this->executeActionRequested(inStr, outStr);
            }
            else
            {
                retVal = -3;
            }

        }
        else
        {
            retVal = -2;
        }

        
    }
    
    return retVal;
}

