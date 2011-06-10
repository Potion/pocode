#pragma once

#include "poEnums.h"
#include "poRect.h"

class poObject;

class poWindow {
public:
	poWindow(const char *title, void *handle, uint root_id, poRect bounds);
	~poWindow();

	void moveTo(poRect rect);
	void fullscreen(bool b);

	std::string title() const;
	int x() const;
	int y() const;
	int width() const;
	int height() const;
	float framerate() const;
	float lastFrameElapsed() const;
	float lastFrameTime() const;
	bool isFullscreen() const;
	poObject* rootObject() const;
	bool wasClosed() const;

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
	
	void *osDependentHandle();

private:
	bool closed_;
	poRect bounds;
	bool fullscreen_;
	
	double last_mark, last_frame;
	int framecounter;
	float framerate_, last_elapsed;
	
	void *handle;
	std::string title_;
	poObject *root;
    poObject *mouse_receiver, *key_receiver, *mouse_hover;
};
