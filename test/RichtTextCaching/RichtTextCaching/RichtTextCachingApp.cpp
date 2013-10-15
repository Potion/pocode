/*	Created by Stephen Varga on 9/18/13.
 *	Copyright 2013 __MyCompanyName__. All rights reserved.
 */

#include "RichtTextCachingApp.h"
#include "poApplication.h"
#include "poTextBox.h"
#include "poCamera.h"

po::TextBox *tb;
po::Font *reg, *italic, *bold, *bold_italic, *con_bold, *huge;

// APP CONSTRUCTOR. Create all objects here.
RichtTextCachingApp::RichtTextCachingApp() {
	addModifier(new po::Camera2D(po::Color::black));
    
    reg         = po::getFont("Courier New",      "Regular");
	italic      = po::getFont("Helvetica Neue",   "Italic");
	bold        = po::getFont("Helvetica Neue",   "Bold");
	bold_italic = po::getFont("Helvetica Neue",   "Bold Italic");
	con_bold    = po::getFont("Helvetica Neue",   "Condensed Bold");
	huge        = po::getFont("Helvetica Neue",   "a");
    
	std::string text =	"<b>ABCD</b>EFGHIJKLM\n"
    "NOPQR<i color='#00f0f0'>STUV</i>WXYZ\n"
    "<a color='#ff0000'>aBp</a><span tracking='1.5'>cdefghi</span>jklm\n"
    "nopqr<bi color='#f0f000'>stuvwxyz</bi>\n"
    "<cb>1234567890</cb> Hello";
    
	//std::string text = "hello world, lets make sure the line doesn't break in the middle of the pepper";
	
	tb = new po::TextBox(300);
	tb->setRichText(true);
	tb->setAlignment(po::ALIGN_BOTTOM_LEFT);
	tb->setTextAlignment(po::ALIGN_LEFT);
	tb->position = po::getWindowCenter();
    tb->setFont(reg);
//	tb->setFont(reg, po::PO_TEXT_REGULAR);
//	tb->setFont(italic, po::PO_TEXT_ITALIC);
//	tb->setFont(bold, po::PO_TEXT_BOLD);
//	tb->setFont(bold_italic, po::PO_TEXT_BOLD_ITALIC);
//	tb->setFont(con_bold, "cb");
//	tb->setFont(huge, "a");
//	tb->setLeading(.85f);
	tb->setText(text);
	tb->setTextSize(60);
	tb->textColor = po::Color(1/255.0f, 123.0f/255.f, 177/255.f);
    //tb->setCacheToTexture(true);
	tb->doLayout();
    
    addChild(tb);
}

// APP DESTRUCTOR. Delete all objects here.
RichtTextCachingApp::~RichtTextCachingApp() {
}

// UPDATE. Called once per frame. Animate objects here.
void RichtTextCachingApp::update() {
	
}

// DRAW. Called once per frame. Draw objects here.
void RichtTextCachingApp::draw() {
	
}

// EVENT HANDLER. Called when events happen. Respond to events here.
void RichtTextCachingApp::eventHandler(po::Event *event) {
	
}

// MESSAGE HANDLER. Called from within the app. Use for message passing.
void RichtTextCachingApp::messageHandler(const std::string &msg, const po::Dictionary& dict, const po::Object *sender) {
	
}
