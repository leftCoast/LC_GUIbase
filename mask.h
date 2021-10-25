#ifndef mask_h
#define mask_h

#include <baseGraphics.h>



// Mask can be applied to a bmpObj to tell whether to write a pixel or not. This can be
// inherited and used for any masking formula you want. Just override checkPixel and have
// at it.
//
// .bmpMask below is a prime example of how this can be used.

class mask {

	public:
				mask(void);
	virtual	~mask(void);
	
	virtual	bool	checkPixel(int x,int y);
};



// A simple rect mask. If maskInside is true, pixels will NOT be drawn inside the rect. If
// false, they will NOT be drawn OUTSIDE of the rect.

class maskRect :	public mask,
						public rect {

	public:
				maskRect(rect* inRect,bool maskInside = true);
				maskRect(int inX, int inY, int inWidth,int inHeight,bool maskInside = true);
	virtual	~maskRect(void);
	
				void	setMaskInside(bool maskInside);
	virtual	bool	checkPixel(int x,int y);
	
				bool	inside;
};

#endif