#include <mask.h>

// *************************************
//     The -tiny- mask base class.
// *************************************


mask::mask(void) { setInverse(false); }
mask::~mask(void) {  }

void mask::setInverse(bool inInverse) { inverse = inInverse; }
masktype	mask::checkRect(rect* inRect) { return unMasked; }
masktype	mask::checkRect(int x,int y,int w,int h) { return unMasked; }
bool mask::checkPixel(int x,int y) { return true; }



// *************************************
//     The simple maskRect class.
// *************************************


maskRect::maskRect(rect* inRect)
	: mask(),
	rect(inRect) { }

	
maskRect::maskRect(int inX, int inY, int inWidth,int inHeight)
	: mask(),
	rect(inX,inY,inWidth,inHeight) { }
	
	
maskRect::~maskRect(void) { }


masktype	maskRect::checkRect(rect* inRect) {
		
	if (inRect->isSubRectOf(this)) {
		if (inverse) {
			return unMasked;
		} else {
			return totalMasked;
		}
	} else if (inRect->overlap(this)) {
		return partialMasked;
	} else {
		if (inverse) {
			return totalMasked;
		} else {
			return unMasked;
		}
	}
}


masktype	maskRect::checkRect(int x,int y,int w,int h) {

	rect	inRect;
	
	inRect.setRect(x,y,w,h);
	return checkRect(&inRect);
}


bool maskRect::checkPixel(int x,int y) {

	if (inverse) 
		return !inRect(x,y);
	else
		return inRect(x,y);
}