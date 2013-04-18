#include "poUITextField.h"

const std::string TextField::TEXT_EDIT_BEGAN_MESSAGE;
const std::string TextField::TEXT_EDIT_DONE_MESSAGE;
const std::string TextField::TEXT_CHANGED_MESSAGE;


//------------------------------------------------------------------
TextField::TextField(float w, float h)
{

	padding = 4;
	
	bg = new poRectShape(w,h);
	bg->strokeColor = poColor::black;
	bg->generateStroke(1);
	addChild(bg);
	bg->addEvent(PO_MOUSE_DOWN_INSIDE_EVENT, this);
	
	text = new poTextBox(w-4);
	text->position = poPoint(padding,4);
	text->position.set(padding, padding, 0);
	addChild(text);
	
	cursor = new poLineShape(poPoint(0,0), poPoint(0,22));
	cursor->strokeColor = poColor::black;
	cursor->generateStroke(1);
	cursor->position = poPoint(padding,4);
	cursor->visible = false;
	addChild(cursor);
	
	init();
}

//------------------------------------------------------------------
TextField::~TextField()
{
}

//------------------------------------------------------------------
TextField& TextField::init(){
	editable = true;
	edit_mode = false;
	last_sec = poGetElapsedTime();
	
	cursor->position.x = padding;
	cursor->visible = false;
	
	hasDefaultText = false;
	defaultText = "Enter text here";
	
	clearText();
}

//------------------------------------------------------------------
TextField& TextField::setEditMode(bool b){
	bool prv_edit_mode;
	edit_mode = b && editable;
	if(edit_mode && prv_edit_mode != edit_mode){
		sendBeganMessage();
	}
}

//------------------------------------------------------------------
TextField& TextField::setEditable(bool enableEdit){
	editable = enableEdit;
}

//------------------------------------------------------------------
bool TextField::isEditable(){
	return editable;
}

//------------------------------------------------------------------
bool TextField::isBeingEdited(){
	return edit_mode;
}

//------------------------------------------------------------------
TextField& TextField::setText(std::string s){
	std::string prv_text = text->getText();
	text->setText(s);
	text->doLayout();
	
	if(text->getTextBounds().width > bg->getWidth()-padding*2 ||
	   text->getTextBounds().height > bg->getHeight()-padding){
		text->setText(prv_text);
		text->doLayout();
	}
	
	cursor->position.x = padding + text->getTextBounds().width + 2;
}

//------------------------------------------------------------------
std::string TextField::getText(){
	return text->getText();
}

//------------------------------------------------------------------
TextField& TextField::clearText(){
	this->setText("");
}

//------------------------------------------------------------------
TextField& TextField::setDefaultText(std::string s){
	this->setText(s);
	defaultText = s;
	hasDefaultText = true;
}

//------------------------------------------------------------------
TextField& TextField::setImage(std::string path){
	bg->placeTexture(poGetTexture(path));
}

//------------------------------------------------------------------
TextField& TextField::setColor(poColor color){
	bg->fillColor = color;
}

//------------------------------------------------------------------
TextField& TextField::setSize(float w, float h){
	bg->reshape(w, h);
	text->reshape(w-padding*2, h-padding*2);
	text->position.set(padding, padding, 0);
}

//------------------------------------------------------------------
TextField& TextField::setPadding(float p){
	padding = p;
}

//------------------------------------------------------------------
TextField& TextField::setCornerRadius(float radius){
	bg->reshape(bg->getWidth(), bg->getHeight(), radius);
}

//------------------------------------------------------------------
TextField& TextField::setStrokeColor(poColor c){
	bg->generateStroke(bg->getStrokeWidth()).strokeColor = c;
}

//------------------------------------------------------------------
TextField& TextField::setStrokeSize(float f){
	bg->generateStroke(f);
}

//------------------------------------------------------------------
TextField& TextField::setTextColor(poColor color){
	text->textColor = color;
}

//------------------------------------------------------------------
TextField& TextField::setFont(poFont* font){
	text->setFont(font);
}

//------------------------------------------------------------------
TextField& TextField::setTextSize(float size){
	text->setTextSize(size);
}

//------------------------------------------------------------------
TextField& TextField::setCursorColor(poColor color){
	cursor->strokeColor = color;
	
}

//------------------------------------------------------------------
TextField& TextField::setCursorBlinkSpeed(float seconds){
	
}

//------------------------------------------------------------------
//	Messages
//------------------------------------------------------------------
#pragma mark Messages

//------------------------------------------------------------------
void TextField::sendDoneMessage() {
	poMessageCenter::broadcastMessage(TEXT_EDIT_DONE_MESSAGE, this);
}


//------------------------------------------------------------------
void TextField::sendBeganMessage() {
	poMessageCenter::broadcastMessage(TEXT_EDIT_BEGAN_MESSAGE, this);
}


//------------------------------------------------------------------
void TextField::sendChangedMessage() {
	poMessageCenter::broadcastMessage(TEXT_CHANGED_MESSAGE, this);
}

//------------------------------------------------------------------
bool TextField::isTextDefaultText() {
	return (text->getText() == defaultText);
}

//------------------------------------------------------------------
void TextField::update(){
	float cur_sec = poGetElapsedTime();
	blink_gap_secs = 0.4;
	if(edit_mode && last_sec + blink_gap_secs < cur_sec){
		cursor->visible = !cursor->visible;
		last_sec = cur_sec;
	}
}

//------------------------------------------------------------------
void TextField::eventHandler(poEvent *event)
{
	if(event->type == PO_MOUSE_DOWN_INSIDE_EVENT){
		if(editable){
			edit_mode = true;
			sendBeganMessage();
			
			if(isTextDefaultText())
				clearText();
		}
	}
}

//------------------------------------------------------------------
void TextField::messageHandler(const std::string &msg, const poDictionary& dict, const poObject *sender)
{
}