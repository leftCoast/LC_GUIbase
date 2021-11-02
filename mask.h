#ifndef mask_h
#define mask_h

#include <baseGraphics.h>



// Mask can be applied to a bmpObj to tell whether to write a pixel or not. This can be
// inherited and used for any masking formula you want. Just override checkPixel and have
// at it.
//
// .bmpMask below is a prime example of how this can be used.


enum masktype { unMasked, totalMasked, partialMasked };


class mask {

	public:
				mask(void);
	virtual	~mask(void);
	
	virtual	void		setInverse(bool inInverse);
	virtual	masktype	checkRect(rect* inRect);
	virtual	masktype	checkRect(int x,int y,int w,int h);
	virtual	bool		checkPixel(int x,int y);
	
				bool	inverse;
};



// A simple rect mask. In normal mode, pixels can be drawn outside of the mask rect. They
// will not be drawn inside the mask rect. In inverse mode, pixels can be drawn inside the
// mask rect. But not outside of it. So many double negatives..

class maskRect :	public mask,
						public rect {

	public:
				maskRect(rect* inRect);
				maskRect(int inX, int inY, int inWidth,int inHeight);
	virtual	~maskRect(void);
				
	virtual	masktype	checkRect(rect* inRect);	
	virtual	masktype	checkRect(int x,int y,int w,int h);
	virtual	bool		checkPixel(int x,int y);
	
				
};

#endif