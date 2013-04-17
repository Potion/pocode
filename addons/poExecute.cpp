/*	Created by Ilan Schifter on 4/16/13.
 *	Copyright 2013 Potion Design LLC . All rights reserved.
 */

#include "poExecute.h"
#include "poMessageCenter.h"

const string poExecute::EXE_DONE   = "DONE_EXECUTING_COMMAND";

//----------------------------------------------------------------------------
poExecute::poExecute(string _command, bool _blocking, string _expected_stdout):
command(_command),
blocking(_blocking),
expected_stdout(_expected_stdout)
{
    exec_count = 0;
}

//----------------------------------------------------------------------------
poExecute::~poExecute()
{
}

//----------------------------------------------------------------------------
bool poExecute::execute(){
    ++exec_count;
    
    bool succeeded = true;
    string data;
    
    if(blocking){
        FILE *stream;
        stream = popen(command.c_str(), "r");
        
        succeeded = stream != NULL;
        
        if(succeeded){
            char buffer[1024];
            
            while ( fgets(buffer, 1024, stream) != NULL )
                data.append(buffer);
            
            if(data != expected_stdout){
                printf("Failed to execute: %s\nSTDOUT: %s\n", command.c_str(), data.c_str());
                succeeded = false;
            } 
        }
        
        pclose(stream);
    }
    else {
        succeeded = popen(command.c_str(), "r") != NULL;
    }

    poDictionary dict;
    dict.set("command", command);
    dict.set("stdout", data);
    dict.set("succeeded", succeeded);
    poMessageCenter::broadcastMessage(EXE_DONE, this, dict);
    
    --exec_count;
    
    return succeeded;
}

//----------------------------------------------------------------------------
void poExecute::update()
{
}

//----------------------------------------------------------------------------
void poExecute::eventHandler(poEvent *event)
{
}

//----------------------------------------------------------------------------
void poExecute::messageHandler(const std::string &msg, const poDictionary& dict, const poObject *sender)
{
}

//----------------------------------------------------------------------------
int poExecute::numCurExecuting(){
    return exec_count;
}

//----------------------------------------------------------------------------
void poExecute::setCommand(string _command){
    command = _command;
}

//----------------------------------------------------------------------------
string poExecute::getCommand(){
    return command;
}

//----------------------------------------------------------------------------
void poExecute::getExpectedStdout(string _expected_stdout){
    expected_stdout = _expected_stdout;
}

//----------------------------------------------------------------------------
string poExecute::getExpectedStdout(){
    return expected_stdout;
}

//----------------------------------------------------------------------------
void poExecute::setBlocking(bool b){
    blocking = b;
}

//----------------------------------------------------------------------------
bool poExecute::isBlocking(){
    return blocking;
}

