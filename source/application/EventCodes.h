#pragma once

enum {
	// key event masks
	KEY_CTRL	= 1,
	KEY_SHIFT	= 2,
	KEY_ALT		= 2 << 1,
	KEY_META	= 2 << 2,
	// mouse event masks
	MOUSE_LEFT	= 2 << 3,
	MOUSE_MIDDLE= 2 << 4,
	MOUSE_RIGHT	= 2 << 5
};

