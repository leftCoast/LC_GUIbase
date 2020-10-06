#include <clipRect.h>						
						
						
clipRect::clipRect(void) { mClipRect = NULL; }
clipRect::~clipRect(void) {  }					
void clipRect::setClipRect(rect* inRect) { mClipRect = inRect; }