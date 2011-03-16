
#include "poWindow.h"
#include "CocoaWindowImplGlue.h"

struct CocoaWindowImpl
:   public poWindow::WindowImpl
{
    CocoaWindowImpl(poWindow *win,
                    poWindowType ty,
                    const std::string &t,
                    int x, int y, int w, int h)
	:	nswindow(NULL)
	,	nsglview(NULL)
    {
        window = win;
        type = ty;
        title = t;
        width = w;
        height = h;
        xpos = x;
        ypos = y;
        is_fullscreen = false;
        is_dragging = false;
        
        static int ID = 0;
        id = ID++;
    }
    
    ~CocoaWindowImpl() {
        destroyNSWindow(nswindow, nsglview);
    }
    
    void initialize() {
		makeNSWindow(window, &nswindow, &nsglview, xpos,ypos,width,height,type);
    }
    
    void moveTo(int x, int y, int w, int h) {
        moveNSWindow(nswindow,nsglview,x,y,w,h);
    }
    
    void setFullscreen(bool b) {
		is_fullscreen = b;
        setNSWindowFullscreen(nswindow,nsglview,b);
    }
	
	void *nswindow, *nsglview;
};

typedef CocoaWindowImpl WINDOW_IMPL_TYPE;