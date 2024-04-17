#include <eventMgr.h>
#include <resizeBuff.h>
#include <screen.h>

//#include <debug.h>

// Our global event manager.
eventMgr 	ourEventMgr;

eventType		mType;
	
	unsigned long	mTouchMs;
	unsigned long	mLastMs;
	unsigned long	mNumMs;
	
	point				mTouchPos;
	point				mLastPos;
	
	int				mXDist;
	int				mYDist;
	float				mDist;
	
	float				mXPixlePerSec;
	float				mYPixlePerSec;
	float				mPixalPerSec;
	
	float				mAngle;

/*	
void printEvent(event* anEvent) {

	
	Serial.print(F("mType         : "));
	switch (anEvent->mType) {
		case nullEvent		: Serial.println(F("nullEvent"));	break;
		case touchEvent	: Serial.println(F("touchEvent"));	break;
		case liftEvent		: Serial.println(F("liftEvent"));	break;
		case dragBegin		: Serial.println(F("dragBegin"));	break;
		case dragOn			: Serial.println(F("dragOn")); 		break;
		case clickEvent	: Serial.println(F("clickEvent"));	break;
	}
	Serial.println();
	
	Serial.print(F("Size in bytes : "));Serial.println(sizeof(event));
	Serial.println();
	
	Serial.print(F("mTouchMs      : "));Serial.println(anEvent->mTouchMs);
	Serial.print(F("mLastMs       : "));Serial.println(anEvent->mLastMs);
	Serial.print(F("mNumMs        : "));Serial.println(anEvent->mNumMs);
	Serial.println();
	
	printPoint(&anEvent->mTouchPos,"mTouchPos     : ");Serial.println();
	printPoint(&anEvent->mLastPos,"mLastPos     : ");Serial.println();
	Serial.println();
	
	Serial.print(F("mXDist        : "));Serial.println(anEvent->mXDist);
	Serial.print(F("mYDist        : "));Serial.println(anEvent->mYDist);
	Serial.print(F("mDist         : "));Serial.println(anEvent->mDist);
	Serial.println();
	
	Serial.print(F("mXPixlePerSec : "));Serial.println(anEvent->mXPixlePerSec);
	Serial.print(F("mYPixlePerSec : "));Serial.println(anEvent->mYPixlePerSec);
	Serial.print(F("mPixalPerSec  : "));Serial.println(anEvent->mPixalPerSec);
	Serial.println();
	
	Serial.print(F("mAngle        : "));Serial.println(anEvent->mAngle);
};
*/


long lAbs(long val) {

	if (val<0) {
		return -val; 
	} else {
		return val;
	}
}
	
	
eventObj::eventObj(event* inEvent) { mEvent = inEvent; }


eventObj::~eventObj(void) { if (mEvent) free(mEvent); };
				


// ***************************************************
// eventMgr, This is the guy that figures out what kind
// of screen things are going on. touch, lift, dragBegin,
// dragOn, click or nothing.
// ***************************************************
   
				
eventMgr::eventMgr(void) {
	
	setTime(DRAG_TIME);		// Set up our timer.
	
	mTouched		= false;		// Initialize the past.
	mDragging	= false;		// This is a lot like rewriting history.
	
	mTouchMs		= 0;
	mLastMs		= 0;
	
	mTouchPos.x	= 0;
	mTouchPos.y	= 0;
	mLastPos.x	= 0;
	mLastPos.y	= 0;
				
	mNullEvent.mType				= nullEvent;	// Setup a global, ready to go null event.						
	mNullEvent.mTouchMs			= 0;				// This way if nothing's going on. We won't	
	mNullEvent.mLastMs			= 0;
	mNullEvent.mNumMs				= 0;
	
	mNullEvent.mTouchPos			= mLastPos;		// waste time creating a bunch of these.						
	mNullEvent.mLastPos			= mLastPos;		// Using mLastPos for all these 'cause its currently 0,0.
								
	mNullEvent.mXDist				= 0;				
	mNullEvent.mYDist				= 0;
	mNullEvent.mDist				= 0;
	
	mNullEvent.mXPixlePerSec	= 0;	
	mNullEvent.mYPixlePerSec	= 0;				
	mNullEvent.mPixalPerSec		= 0;
	
	mNullEvent.mAngle				= 0;
}


eventMgr::~eventMgr(void) { }


void eventMgr::begin(void) { hookup(); }


// Sometimes you want a fresh start.
void eventMgr::flushEvents(void) { dumpList(); }


// We have an event for somebody?
bool eventMgr::haveEvent(void) { return !isEmpty(); }


// Someone wants the next event. That's our job!
event eventMgr::getEvent(void) {

	eventObj*	nextEvent;
	event			anEvent;
	
	nextEvent = (eventObj*)pop();			// Pop the queue for a pointer to the next event.
	if (nextEvent) {							// If there was an event to pop off the queue..
		anEvent = *(nextEvent->mEvent);	// Save off the data to stack memory.
		delete nextEvent;						// Delete the copy we pulled off the queue.
		return anEvent;						// Return our local copy.
	}
	return mNullEvent;						// No event waiting? Send them a nullEvent.
}


bool eventMgr::active(void) {

	return  !isEmpty() || mTouched;
}


// We figure out every possible bit of information we
// can about an event.
void eventMgr::addEvent(eventType inType) {

	event*		newEvent;		// A pointer to where the event info will live.
	eventObj*	newEventObj;	// A pointer to the even object that will live in our queue and HOLD the pointer to the event info.. Wow!
	//Serial.println("add event");
	newEvent = NULL;															// The pointer needs to start as NULL.
	if (resizeBuff(sizeof(event),(uint8_t**)&newEvent)) {			// Becuse we allocate with resizeBuff();
		switch(inType) {														// Check the flavor..
			case nullEvent		: free(newEvent); return;				// We don't make nullEvents, we recycle a const one.
			case touchEvent	:												// Touch event?
				newEvent->mType			= touchEvent;					// Set the type.
				newEvent->mTouchMs		= mTouchMs;						// Fist thing, grab the time.					
				newEvent->mLastMs			= mTouchMs;						// Same time, we just touched.
				newEvent->mNumMs			= 0;								// Calculate the number of milliseconds since last time.

				newEvent->mTouchPos		= mTouchPos;					// Grab the location of the event.					
				newEvent->mLastPos		= mTouchPos;					// And everything else we can.
								
				newEvent->mXDist			= 0;				
				newEvent->mYDist			= 0;
				newEvent->mDist			= 0;
	
				newEvent->mXPixlePerSec	= 0;	// LINE 190
				newEvent->mYPixlePerSec	= 0;				
				newEvent->mPixalPerSec	= 0;
	
				newEvent->mAngle			= 0;
			break;
			case liftEvent		:																			// liftEvent?
				newEvent->mType			= liftEvent;												// Set the type.
				newEvent->mTouchMs		= mTouchMs;													// When it all started.
				newEvent->mLastMs			= millis();													// mLastMs - Basically the lift time.
				newEvent->mNumMs			= lAbs(newEvent->mLastMs-mTouchMs);					// Calculate the number of milliseconds since touch.
	
				newEvent->mTouchPos		= mTouchPos;												// Saved when we got the touch.					
				newEvent->mLastPos		= mLastPos;													// Updated during idle time while touched.
								
				newEvent->mXDist			= xDistance(mTouchPos,mLastPos);						// Calculate the x distance.				
				newEvent->mYDist			= yDistance(mTouchPos,mLastPos);						// Calculate the y distance.	
				newEvent->mDist			= distance(mTouchPos,mLastPos);						// Calculate the total distance.	
	
				newEvent->mXPixlePerSec	= (newEvent->mXDist/newEvent->mNumMs)*1000;		// Calculate the x speed. Pixels/sec.
				newEvent->mYPixlePerSec	= (newEvent->mYDist/newEvent->mNumMs)*1000;		// Calculate the y speed. Pixels/sec.				
				newEvent->mPixalPerSec	= (newEvent->mDist/newEvent->mNumMs)*1000;		// Calculate the total speed. Pixels/sec.
	
				newEvent->mAngle			= angle(mTouchPos,mLastPos);							// Calculate the actual angle, in radians.
			break;
			case dragBegin		:																			// dragBegin?
				newEvent->mType			= dragBegin;												// Set the type.
				newEvent->mTouchMs		= mTouchMs;													// When it all started.
				newEvent->mLastMs			= millis();													// What time is it now?
				newEvent->mNumMs			= lAbs(newEvent->mLastMs-mTouchMs);					// Calculate the number of milliseconds since touch.
	
				newEvent->mTouchPos		= mTouchPos;												// Saved when we got the touch.					
				newEvent->mLastPos		= mLastPos;													// Updated during idle time while touched.
								
				newEvent->mXDist			= xDistance(mTouchPos,mLastPos);						// Calculate the x distance.				
				newEvent->mYDist			= yDistance(mTouchPos,mLastPos);						// Calculate the y distance.	
				newEvent->mDist			= distance(mTouchPos,mLastPos);						// Calculate the total distance.	
	
				newEvent->mXPixlePerSec	= (newEvent->mXDist/newEvent->mNumMs)*1000;		// Calculate the x speed. Pixels/sec.
				newEvent->mYPixlePerSec	= (newEvent->mYDist/newEvent->mNumMs)*1000;		// Calculate the y speed. Pixels/sec.				
				newEvent->mPixalPerSec	= (newEvent->mDist/newEvent->mNumMs)*1000;		// Calculate the total speed. Pixels/sec.
	
				newEvent->mAngle			= angle(mTouchPos,mLastPos);							// Calculate the actual angle, in radians.
			break;
			case dragOn			:
				newEvent->mType			= dragOn;					
				newEvent->mTouchMs		= mTouchMs;					
				newEvent->mLastMs			= millis();
				newEvent->mNumMs			= lAbs(newEvent->mLastMs-mTouchMs);					// Calculate the number of milliseconds since touch.
	
				newEvent->mTouchPos		= mTouchPos;												// Saved when we got the touch.					
				newEvent->mLastPos		= mLastPos;													// Updated during idle time while touched.
								
				newEvent->mXDist			= xDistance(mTouchPos,mLastPos);						// Calculate the x distance.				
				newEvent->mYDist			= yDistance(mTouchPos,mLastPos);						// Calculate the y distance.	
				newEvent->mDist			= distance(mTouchPos,mLastPos);						// Calculate the total distance.	
	
				newEvent->mXPixlePerSec	= (newEvent->mXDist/newEvent->mNumMs)*1000;		// Calculate the x speed. Pixels/sec.
				newEvent->mYPixlePerSec	= (newEvent->mYDist/newEvent->mNumMs)*1000;		// Calculate the y speed. Pixels/sec.				
				newEvent->mPixalPerSec	= (newEvent->mDist/newEvent->mNumMs)*1000;		// Calculate the total speed. Pixels/sec.
	
				newEvent->mAngle			= angle(mTouchPos,mLastPos);							// Calculate the actual angle, in radians.
			break;
			case clickEvent	:
				newEvent->mType			= clickEvent;					
				newEvent->mTouchMs		= mTouchMs;					
				newEvent->mLastMs			= millis();
				newEvent->mNumMs			= lAbs(newEvent->mLastMs-mTouchMs);					// Calculate the number of milliseconds since touch.
	
				newEvent->mTouchPos		= mTouchPos;												// Saved when we got the touch.					
				newEvent->mLastPos		= mLastPos;													// Updated during idle time while touched.
								
				newEvent->mXDist			= xDistance(mTouchPos,mLastPos);						// Calculate the x distance.				
				newEvent->mYDist			= yDistance(mTouchPos,mLastPos);						// Calculate the y distance.	
				newEvent->mDist			= distance(mTouchPos,mLastPos);						// Calculate the total distance.	
	
				newEvent->mXPixlePerSec	= (newEvent->mXDist/newEvent->mNumMs)*1000;		// Calculate the x speed. Pixels/sec.
				newEvent->mYPixlePerSec	= (newEvent->mYDist/newEvent->mNumMs)*1000;		// Calculate the y speed. Pixels/sec.				
				newEvent->mPixalPerSec	= (newEvent->mDist/newEvent->mNumMs)*1000;		// Calculate the total speed. Pixels/sec.
	
				newEvent->mAngle			= angle(mTouchPos,mLastPos);							// Calculate the actual angle, in radians.
			break;
		}
		newEventObj = new eventObj(newEvent);														// Create the node this will live in.
		if (newEventObj) { push(newEventObj); }													// We are a queue. Push the new guy in.
	}
}


// Use this so we can have some type checking.
void eventMgr::push(eventObj* newEventObj) { queue::push((linkListObj*)newEventObj); }


// And this. I was bit by doing it fast and loose earlier.
eventObj* eventMgr::pop(void) { return (eventObj*)queue::pop(); }


// Grab touch and drag moves from the finger. ALL of this is in global coordinates.
void eventMgr::idle(void) {

	float	moveDist;
	
	if (screen->touched()) {									// If we've been touched! Or, are still touched..
		//Serial.println("touch!");
		mLastPos = screen->getPoint();						//	Update the last point we saw.
		if (mTouched) {											// If last time we checked we were mTouched.
			moveDist = distance(mTouchPos,mLastPos);		// Calculate the total distance from initial touch.
			if (mDragging) {										// If we're already dragging..
				if (isEmpty()) {									// If we have an empty queue.
 					addEvent(dragOn);								// We pop in a dragOn event. Don't swamp the queue.
 				}														// 
			} else if (ding()||moveDist>DRAG_DIST) {		// If our drag timer expired, or were moving, it's a drag.	
				mDragging = true;									// Note that we are mDragging.
				addEvent(dragBegin);								// Create a drag begin event.
			}															//
		} else {														// Else, this is initial contact!
			mTouchMs = millis();									// Save touch time..
			mTouchPos = mLastPos;								// Grab our initial location.
			mTouched = true;										// Note that we've been mTouched. (Unwanted personal contact!)
			start();													// Start up the drag timer.
			addEvent(touchEvent);								// Create a touch event.
		}																//
	} else {															// Else, we are not being touched. (Now who's feeling lonely?)
		if (mTouched) {											// Last time we checked we were mTouched.
			mTouched = false;										// Save off that we're no longer mTouched.
			mDragging = false;									// No matter, we're not mDragging.
			addEvent(liftEvent);									// This means we got a lift.
			if (!ding()) {											// If it was a "short" touch.
 				addEvent(clickEvent);							// That'll pass for a "click".
 			}
 		}
 	}
 }

