#pragma once

#include "poObject.h"

class poWindow {
public:
	poWindow(const char *title, uint root_id, poRect bounds);
	~poWindow();

	void moveTo(poPoint p);
	void fullscreen(bool b);

	std::string title() const;
	int x() const;
	int y() const;
	int width() const;
	int height() const;
	poRect frame() const;
	poRect bounds() const;
	poPoint centerPoint() const;
	float framerate() const;
	int framecount() const;
	float lastFrameElapsed() const;
	float lastFrameTime() const;
	bool isFullscreen() const;
	poObject* rootObject() const;
	bool wasClosed() const;
	
	poPoint mousePosition() const;

	void makeCurrent();
	void update();
	void draw();

	void mouseDown(int x, int y, int mod);
	void mouseUp(int x, int y, int mod);
	void mouseMove(int x, int y, int mod);
	void mouseDrag(int x, int y, int mod);
	void mouseWheel(int x, int y, int mod, int num_steps);
	void keyDown(int key, int code, int mod);
	void keyUp(int key, int code, int mod);
	void resized(int w, int h);
	void resized(int x, int y, int w, int h);
	
	void touchBegin(int x, int y, int ID );
	void touchMove(int x, int y, int ID );
	void touchEnd(int x, int y, int ID );
	
	void *getWindowHandle();
	void setWindowHandle(void *handle);

	int nextDrawOrder();

private:
	bool closed_;
	poRect _bounds;
	bool fullscreen_;
	
	double last_mark, last_frame;
	int framecounter, total_framecount;
	float framerate_, last_elapsed;
	
	void *handle;
	std::string title_;
	poObject *root;
	
    poObject *key_receiver, *drag_target;
	std::set<poObject*> mouse_hovers;
	
	// the global mouse position
	poPoint mouse_pos;
	
	int draw_order_counter;
	
	// store incoming events here
	std::deque<poEvent> received;
	// then process them on the main thread
	void processEvents();
};
