#include <mask.h>

// *************************************
//     The -tiny- mask base class.
// *************************************


mask::mask(void) {  }
mask::~mask(void) {  }
bool mask::checkPixel(int x,int y) { return true; }



// *************************************
//     The simple maskRect class.
// *************************************


maskRect::maskRect(rect* inRect,bool maskInside)
	: mask(),
	rect(inRect) { 
	
	setMaskInside(maskInside); 
}

	
maskRect::maskRect(int inX, int inY, int inWidth,int inHeight,bool maskInside)
	: mask(),
	rect(inX,inY,inWidth,inHeight) {
	
	setMaskInside(maskInside);
}
	
	
maskRect::~maskRect(void) {  }


void maskRect::setMaskInside(bool maskInside) { inside = maskInside; }


bool maskRect::checkPixel(int x,int y) {

	if (inside) {
		return !inRect(x,y);
	} else {
		return inRect(x,y);
	}
}