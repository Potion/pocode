#pragma once

#include <list>
#include "poWindow.h"

class poApplication 
:	public boost::noncopyable
{
public:
	static poApplication *get() {
		static poApplication *instance = NULL;
		if(!instance)
			instance = new poApplication();
		return instance;
	}
	
	// kill all windows and the application
	void quit() {
		BOOST_FOREACH(poWindow *win, windows) {
			delete win;
		}
		windows.clear();
	}
	
	int numWindows() const {
		return (int)windows.size();
	}
	
	void addWindow(poWindow* window) {
		windows.insert(window);
	}
	
	void removeWindow(poWindow* window) {
		windows.erase(window);
	}
	
	// get window with id
	poWindow* getWindow(int id) {
		BOOST_FOREACH(poWindow *win, windows) {
			if(win->getId() == id)
				return win;
		}
		return NULL;
	}
	
	double framerate;
	std::set<poWindow*> windows;
	poWindow *currentWindow;

private:
	explicit poApplication()
	:	framerate(60.0)
	{}
	
    virtual ~poApplication() {}
};

inline int getWindowWidth() {
	return poApplication::get()->currentWindow->getWidth();
}

inline int getWindowHeight() {
	return poApplication::get()->currentWindow->getHeight();
}

inline int getWindowWidth(int win_id) {
	return poApplication::get()->getWindow(win_id)->getWidth();
}

inline int getWindowHeight(int win_id) {
	return poApplication::get()->getWindow(win_id)->getHeight();
}

extern void setupApplication();
extern void cleanupApplication();

