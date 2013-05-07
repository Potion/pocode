#pragma once

/*	Created by Ilan Schifter on 4/16/13.
 *	Copyright 2013 Potion Design LLC . All rights reserved.
 */


#include "poObject.h"
#include "boost/thread.hpp"

using namespace std;

class poExecute : public poObject
{
public:
	poExecute(string _command, bool _blocking = false, string _expected_stdout = "");
	virtual ~poExecute();
	
	virtual void update();
	virtual void eventHandler(poEvent *event);
	virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary(), const poObject *sender=NULL);
    
    virtual bool execute();
    
    
    int		numCurExecuting();
    
	void	setCommand(string _command);
    string	getCommand();
    
	void	getExpectedStdout(string _expected_stdout);
    string	getExpectedStdout();
    
	void	setBlocking(bool b);
    bool	isBlocking();
    
    static const std::string EXE_DONE;
    
protected:
    string	command;
    string	expected_stdout;
    bool	blocking;
    int		exec_count;
};