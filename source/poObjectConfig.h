#pragma once

#include "poShape2D.h"
#include "poTextBox.h"

template <typename T=poObject>
class poObjectConfig {
protected:
	typedef T ObjT;
	T *obj;
	
public:
	poObjectConfig(ObjT *obj) : obj(obj)				{}
	
	T					*object()						{return obj;}
	poObjectConfig<T>	&name(const std::string &str)	{obj->name = str; return *this;}
	poObjectConfig<T>	&position(poPoint p)			{obj->position = p; return *this;}
	poObjectConfig<T>	&scale(poPoint p)				{obj->scale = p; return *this;}
	poObjectConfig<T>	&rotation(float r)				{obj->rotation = r; return *this;}
	poObjectConfig<T>	&rotationAxis(poPoint p)		{obj->rotationAxis = p; return *this;}
	poObjectConfig<T>	&offset(poPoint p)				{obj->offset = p; return *this;}
	poObjectConfig<T>	&visible(bool b)				{obj->visible = b; return *this;}
	poObjectConfig<T>	&alignment(poAlignment a)		{obj->alignment(a); return *this;}
	poObjectConfig<T>	&matrixOrder(poMatrixOrder ord)	{obj->matrixOrder = ord; return *this;}
};

class poShape2DConfig : public poObjectConfig<poShape2D> {
public:
	poShape2DConfig(poShape2D *obj) : poObjectConfig<poShape2D>(obj){}

	poShape2DConfig		&fillDrawStyle(GLenum e)	{obj->fillDrawStyle = e; return *this;}
	poShape2DConfig		&fillColor(poColor c)		{obj->fillColor = c; return *this;}
	poShape2DConfig		&strokeColor(poColor c)		{obj->strokeColor = c; return *this;}
	poShape2DConfig		&fillEnabled(bool b)		{obj->fillEnabled = b; return *this;}
	poShape2DConfig		&strokeEnabled(bool b)		{obj->strokeEnabled = b; return *this;}
	poShape2DConfig		&useSimpleStroke(bool b)	{obj->useSimpleStroke = b; return *this;}
	poShape2DConfig		&closed(bool b)				{obj->closed = b; return *this;}
	poShape2DConfig		&drawBounds(bool b)			{obj->drawBounds = b; return *this;}
	poShape2DConfig		&alphaTestTexture(bool b)	{obj->alphaTestTexture = b; return *this;}
};

class poTextBoxConfig : public poObjectConfig<poTextBox> {
public:
	poTextBoxConfig(poTextBox *obj) : poObjectConfig<poTextBox>(obj){}
	
	poTextBoxConfig		&text(const std::string& s)	{obj->text(s); return *this;}
	poTextBoxConfig		&textColor(poColor c)		{obj->textColor = c; return *this;}
	poTextBoxConfig		&textAlignment(poAlignment a) {obj->textAlignment(a); return *this;}
	poTextBoxConfig		&font(poFont* f, const std::string &weight=PO_FONT_REGULAR) {obj->font(f,weight); return *this;}
	poTextBoxConfig		&leading(float f)			{obj->leading(f); return *this;}
	poTextBoxConfig		&tracking(float f)			{obj->tracking(f); return *this;}
	poTextBoxConfig		&padding(float f)			{obj->padding(f); return *this;}
	poTextBoxConfig		&padding(float h, float v)	{obj->padding(h,v); return *this;}
	poTextBoxConfig		&padding(float l, float r,
									 float t, float b)	{obj->padding(l,r,t,b); return *this;}
	poTextBoxConfig		&drawBounds(bool b)			{obj->drawBounds = b; return *this;}
};


