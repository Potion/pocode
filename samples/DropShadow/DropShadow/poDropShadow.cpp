#include "poDropShadow.h"
#include "poResourceStore.h"

poDropShadow::poDropShadow( poPoint dimensions, float spread, float alpha )
{
	top_left = new poRectShape( spread, spread );
	top_center = new poRectShape( dimensions.x, spread );
	top_right = new poRectShape( spread, spread );
	center_left = new poRectShape( spread, dimensions.y );
	center_center = new poRectShape( dimensions.x, dimensions.y );
	center_right = new poRectShape( spread, dimensions.y );
	bottom_left = new poRectShape( spread, spread );
	bottom_center = new poRectShape( dimensions.x, spread );
	bottom_right = new poRectShape( spread, spread );
	
	poImage* tex;
	
	tex = getImage("dropShadowAssets/dropShadow_top_left.png");
	top_left->alignment( PO_ALIGN_BOTTOM_RIGHT );
	top_left->position = poPoint( 0.f, 0.f );
	top_left->placeTexture( tex->texture(), PO_TEX_FIT_EXACT);
	top_left->alpha = alpha;
	
	tex = getImage("dropShadowAssets/dropShadow_top_center.png");
	top_center->alignment( PO_ALIGN_BOTTOM_LEFT );
	top_center->position = poPoint( 0.f, 0.f );
	top_center->placeTexture( tex->texture(), PO_TEX_FIT_EXACT);
	top_center->alpha = alpha;
	
	tex = getImage("dropShadowAssets/dropShadow_top_right.png");
	top_right->alignment( PO_ALIGN_BOTTOM_LEFT );
	top_right->position = poPoint( dimensions.x, 0.f );
	top_right->placeTexture( tex->texture(), PO_TEX_FIT_EXACT);
	top_right->alpha = alpha;
	
	tex = getImage("dropShadowAssets/dropShadow_center_left.png");
	center_left->alignment( PO_ALIGN_TOP_RIGHT );
	center_left->position = poPoint( 0.f, 0.f );
	center_left->placeTexture( tex->texture(), PO_TEX_FIT_EXACT);
	center_left->alpha = alpha;
	
	center_center->fillColor = poColor::black;
	center_center->alignment( PO_ALIGN_TOP_LEFT );
	center_center->position = poPoint( 0.f, 0.f );
	center_center->alpha = alpha;
	
	tex = getImage("dropShadowAssets/dropShadow_center_right.png");
	center_right->alignment( PO_ALIGN_TOP_LEFT );
	center_right->position = poPoint( dimensions.x, 0.f );
	center_right->placeTexture( tex->texture(), PO_TEX_FIT_EXACT);
	center_right->alpha = alpha;
	
	tex = getImage("dropShadowAssets/dropShadow_bottom_left.png");
	bottom_left->alignment( PO_ALIGN_TOP_RIGHT );
	bottom_left->position = poPoint( 0.f, dimensions.y );
	bottom_left->placeTexture( tex->texture(), PO_TEX_FIT_EXACT);
	bottom_left->alpha = alpha;
	
	tex = getImage("dropShadowAssets/dropShadow_bottom_center.png");
	bottom_center->alignment( PO_ALIGN_TOP_LEFT );
	bottom_center->position = poPoint( 0.f, dimensions.y );
	bottom_center->placeTexture( tex->texture(), PO_TEX_FIT_EXACT);
	bottom_center->alpha = alpha;
	
	tex = getImage("dropShadowAssets/dropShadow_bottom_right.png");
	bottom_right->alignment( PO_ALIGN_TOP_LEFT );
	bottom_right->position = poPoint( dimensions.x, dimensions.y );
	bottom_right->placeTexture( tex->texture(), PO_TEX_FIT_EXACT);
	bottom_right->alpha = alpha;
	
	addChild(top_left);
	addChild(top_center);
	addChild(top_right);
	addChild(center_left);
	addChild(center_center);
	addChild(center_right);
	addChild(bottom_left);
	addChild(bottom_center);
	addChild(bottom_right);
	
}

poDropShadow::~poDropShadow() 
{
}

void poDropShadow::update() 
{
}

void poDropShadow::eventHandler(poEvent *event)
{
}

void poDropShadow::messageHandler(const std::string &msg, const poDictionary& dict) 
{
}

void poDropShadow::setAlpha(float alpha){
	
	top_left->alpha = alpha;
	top_center->alpha = alpha;
	top_right->alpha = alpha;
	center_left->alpha = alpha;
	center_center->alpha = alpha;
	center_right->alpha = alpha;
	bottom_left->alpha = alpha;
	bottom_center->alpha = alpha;
	bottom_right->alpha = alpha;
}

void poDropShadow::setSpread( poPoint dimensions, float spread){

	top_left->reshape( spread, spread );
	top_center->reshape( dimensions.x, spread );
	top_right->reshape( spread, spread );
	center_left->reshape( spread, dimensions.y );
	center_center->reshape( dimensions.x, dimensions.y );
	center_right->reshape( spread, dimensions.y );
	bottom_left->reshape( spread, spread );
	bottom_center->reshape( dimensions.x, spread );
	bottom_right->reshape( spread, spread );

}
