/*	Created by Steve Varga on 4/10/12.
 *	Copyright 2012 Vargatron. All rights reserved.
 */

#include "RichTextExampleApp.h"
#include "poApplication.h"
#include "poCamera.h"
#include "poTextBox.h"

poTextBox *tb;
poFont *reg, *italic, *bold, *bold_italic, *con_bold, *huge;


// APP CONSTRUCTOR. Create all objects here.
RichTextExampleApp::RichTextExampleApp() {
	addModifier(new poCamera2D(poColor::black));
    
    reg         = poGetFont("Courier New",      "Regular");
	italic      = poGetFont("Helvetica Neue",   "Italic");
	bold        = poGetFont("Helvetica Neue",   "Bold");
	bold_italic = poGetFont("Helvetica Neue",   "Bold Italic");
	con_bold    = poGetFont("Helvetica Neue",   "Condensed Bold");
	huge        = poGetFont("Helvetica Neue",   "a");
    
	std::string text =	"<b>ABCD</b>EFGHIJKLM\n"
    "NOPQR<i color='#00f0f0'>STUV</i>WXYZ\n"
    "<a color='#ff0000'>aBp</a><span tracking='1.5'>cdefghi</span>jklm\n"
    "nopqr<bi color='#f0f000'>stuvwxyz</bi>\n"
    "<cb>1234567890</cb> Hello";
    
	//std::string text = "hello world, lets make sure the line doesn't break in the middle of the pepper";
	
	tb = new poTextBox(300);
	tb->setRichText(true);
	tb->setAlignment(PO_ALIGN_BOTTOM_LEFT);
	tb->setTextAlignment(PO_ALIGN_LEFT);
	tb->position = getWindowCenter();
	tb->setFont(reg, PO_TEXT_REGULAR);
	tb->setFont(italic, PO_TEXT_ITALIC);
	tb->setFont(bold, PO_TEXT_BOLD);
	tb->setFont(bold_italic, PO_TEXT_BOLD_ITALIC);
	tb->setFont(con_bold, "cb");
	tb->setFont(huge, "a");
	tb->setLeading(.85f);
	tb->setText(text);
	tb->setTextSize(60);
	tb->textColor = poColor::white;
	tb->doLayout();
    
    
    addChild(tb);

}

// APP DESTRUCTOR. Delete all objects here.
RichTextExampleApp::~RichTextExampleApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void RichTextExampleApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void RichTextExampleApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void RichTextExampleApp::eventHandler(poEvent *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void RichTextExampleApp::messageHandler(const std::string &msg, const poDictionary& dict) {
	
}
