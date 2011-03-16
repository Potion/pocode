#pragma once

#include "poEnums.h"

class poObject;

class poWindow {
public:
	poWindow(poObject *root, 
			 poWindowType type,
		     const std::string &title,
		     int x, int y, int w, int h);

	~poWindow();

	// called when the user moves the window
	void setSize(int w, int h);
	void setPosition(int x, int y);
	// move or resize the window in code
	void moveTo(int x, int y, int w, int h);
	void setFullscreen(bool b);
	void toggleFullscreen();
	void setDragging(bool b);
	void setRootObject(poObject *obj);

	std::string getTitle();
	int getWidth();
	int getHeight();
	int getX();
	int getY();
	bool isFullscreen();
	bool isDragging();
	int getId();
	poObject* getRootObject();

	void setup();
	void cleanup();

	void update();
	void draw();

	void mouseDown(int x, int y, int mod);
	void mouseUp(int x, int y, int mod);
	void mouseMove(int x, int y, int mod);
	void mouseDrag(int x, int y, int mod);
	void mouseWheel(int x, int y, int mod, int num_steps);
	void keyDown(char key, int code, int mod);
	void keyUp(char key, int code, int mod);

	struct WindowImpl {
		virtual ~WindowImpl() {}
		virtual void initialize() = 0;
		virtual void moveTo(int x, int y, int w, int h) = 0;
		virtual void setFullscreen(bool b) = 0;

		poWindow *window;
		std::string title;
		int width, height;
		int xpos, ypos;
		bool is_fullscreen;
		bool is_dragging;
		poWindowType type;
		int id;
	};
	WindowImpl *impl;
	poObject *root;
    poObject *mouse_receiver, *key_receiver, *mouse_hover;
};
