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
		using namespace std;
		list<poWindow*>::iterator iter = windows.begin();
		while(iter != windows.end()) {
			poWindow *window = *iter;
			// move the iterator before deleting the window
			iter = windows.erase(iter++);
			delete window;
		}
	}
	
	int numWindows() const {
		return (int)windows.size();
	}
	
	void addWindow(poWindow* window) {
		using namespace std;
		if(find(windows.begin(), windows.end(), window) == windows.end()) {
			windows.push_back(window);
		}
	}
	
	void removeWindow(poWindow* window) {
		windows.remove(window);
	}
	
	// get window with id
	poWindow* getWindow(int id) {
		using namespace std;
		for(list<poWindow*>::iterator iter=windows.begin(); iter!=windows.end(); iter++) {
			poWindow *window = *iter;
			if(window->getId() == id)
				return window;
		}
		return NULL;
	}

	double framerate;
	std::list<poWindow*> windows;

private:
	explicit poApplication()
	:	framerate(60.0)
	{}
	
    virtual ~poApplication() {}
};

inline int getWindowWidth(int win_id) {
	return poApplication::get()->getWindow(win_id)->getWidth();
}

inline int getWindowHeight(int win_id) {
	return poApplication::get()->getWindow(win_id)->getHeight();
}

extern void setupApplication();
extern void cleanupApplication();

