#ifndef drawObj_h
#define drawObj_h

#include <lists.h>
#include <idlers.h>
#include <baseGraphics.h>
#include <displayObj.h>
#include <eventMgr.h>

// ***************************************************************
// Base class for an object that can be drawn on the screen.
// Possibly clicked on.
// ***************************************************************


enum eventSet	{ noEvents, touchLift, fullClick, dragEvents, touchNDrag };


class drawObj : public rect, public dblLinkListObj {


	public:
  				drawObj();
  				drawObj(rect* inRect,eventSet inEventSet=noEvents);
  				drawObj(int inLocX, int inLocY, int inWidth,int inHeight,eventSet inEventSet=noEvents);
	virtual	~drawObj();
    
   virtual	bool		wantRefresh(void);									// Called by manager to see if I need to draw now.
   virtual	void		setNeedRefresh(bool=true);							// Called to cause a redraw. (Used a LOT)
   			void		aTouchAbove(void);									// I've changed. Going to draw. Everyone above, draw too.
	virtual	void		setLocation(int x,int y);							// Changed our mind, move it over here..
   virtual	void  	draw(void);												// Call this one. Don't inherit this one.
   virtual	void		eraseSelf(void);										// Mostly you can ignore this one. Used for animation.
   virtual 	void  	drawSelf(void);										// Inherit this one and make it yours.
	virtual	void		setThisFocus(bool setLoose);						// We are either getting or loosing focus.
	virtual	bool		haveFocus(void);										// Do we have focus or not?
	virtual	void		setEventSet(eventSet inEventSet);				// Want to change our event set on the fly?
	virtual	eventSet	getEventSet(void);									// Want to see what this guys even set is?
  	virtual	bool		acceptEvent(event* inEvent,point* localPt);	// Is this event for us?
  	virtual	void  	doAction(void);										// Override me for action!
	virtual	void  	doAction(event* inEvent,point* localPt);		//	Special for them that drag around.
				void		setCallback(void(*funct)(void));					// Or use a callback.
  
protected:
				bool		needRefresh;											// I need to draw. When its time.
				bool		focus;													// The user is lookin' at me!
				eventSet	mEventSet;												// Things that I respond to.
				bool		clicked;													// Oh Lord! She's touching me! (Touchscreen)
				void		(*callback)(void);									// Function to call when touched. (Optional)
				int		lastX;													// Yes, these are where we were,
				int		lastY;													// before we moved.
};


// ***************************************************************
// Here we have the management of the screen. Get the clicks,
// find drawObjs to act on the clicks and tell the objects when to
// redraw themselves.
//
// "viewList" is the global that does all these things during
// idle time.
// ***************************************************************


class viewMgr : public idler {
    
public:
            viewMgr(void);
	virtual	~viewMgr(void);
    
	virtual	void		addObj(drawObj* newObj);
    			void		dumpDrawObjList(void);
            bool		checkEvents(event* theEvent);
	virtual	void    	checkRefresh(void);
				int		numObjects(void);
				drawObj*	getObj(int index);
				drawObj*	theList(void);
				void		touchAllAbove(drawObj* fromMe);
    virtual void    	idle(void);
    
            drawObj	listHeader;				// Header of the drawObj list;
};

extern	viewMgr		viewList;								// Our global GUI manager.
extern 	drawObj*		theTouched;								// Who's accepted a finger touch on the screen?
extern	drawObj*		currentFocus;							// Focus goes hand in hand with view management.
extern 	void			setFocusPtr(drawObj* newFocus);	// Anyone can set focus by calling this function.
extern 	bool			drawing;									// This is ALWAYS true. WTF?!

// ***************************************************************
// The idea here is to have a draw object that manages a group of
// drawObjects. This is the base of all lists & popups. Or at least,
// that's the plan.
// ***************************************************************

class drawGroup : public drawObj, public viewMgr {

	public:
				drawGroup(rect* inRect,eventSet inEventSet=noEvents);
				drawGroup(int x, int y, int width,int height,eventSet inEventSet=noEvents);
  	virtual	~drawGroup();

	virtual	bool		checkGroupRefresh(void);
	virtual	void		setLocation(int x,int y);
	virtual	void		setGroupRefresh(bool refresh=true);
	virtual	bool		wantRefresh(void);
	virtual	void		setNeedRefresh(bool refresh=true);
  	virtual	bool		acceptEvent(event* inEvent,point* localPt);	// Is this event for us?
  	virtual	void  	addObj(drawObj* newObj);
  	virtual	void  	draw(void);
  	virtual	void    	idle(void);
};			
		
		
		
class drawList : public drawGroup {
 
	public:
				drawList(rect* inRect,eventSet inEventSet=noEvents,bool vertical=true);
				drawList(int x, int y, int width,int height,eventSet inEventSet=noEvents,bool vertical=true);
  	virtual	~drawList();
  					
  	virtual	void		addObj(drawObj* newObj);
  				void		setPositions(int offset=0);
  				int		lastY(void);
  				bool		isVisible(drawObj* theItem);
				void		showItem(drawObj* theItem);
				drawObj*	findItem(point* where);
				
  				int	itemHeight;
  				int	itemWidth;
  				bool	mVertical;
  	
};
	
#endif

