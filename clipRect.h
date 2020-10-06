#ifndef clipRect_h
#define clipRect_h

#include <baseGraphics.h>


class clipRect {

	public:
						clipRect(void);
	virtual			~clipRect(void);
					
	virtual	void	setClipRect(rect* inRect);
	
				rect*	mClipRect;
};
	
#endif