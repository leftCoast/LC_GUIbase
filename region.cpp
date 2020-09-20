#include "region.h"

// Create an empty region.
region::region(void)
	: linkList() { }


// Delete a region..
region::~region(void) { }


// We do what we need to do to add a rectangle to our region. Keeping enclosingRect up to
// date as we do it.
void region::addRect(rect* inRect) {

	regionRectObj*	newRectObj;
	
	if (inRect) {												// Sanity, if they really gave us a valid pointer..
		newRectObj = new regionRectObj(inRect);		// Create a shing new rect object using inRect as template.
		if (newRectObj) {										// If we got the new rectObj..
			if (isEmpty()) {									// If this is our first rect..
				enclosingRect.setRect(inRect);			// Set the enclosing rect to inRect.
			} else {												// Else NOT our first rect..
				enclosingRect.addRect(newRectObj);		// We add inRect to our enclosing rect.
			}
			addToTop(newRectObj);							// We add it to the top of the list.
		}
	}
}


// Add a region to our region. This only copies the inputed region. Does not change it in
// any way.
void region::addRegion(region* inRegion) {

	regionRectObj*	trace;
	
	if (inRegion) {
		trace = (regionRectObj*)inRegion->getFirst();
		while(trace) {
			addRect(trace);
			trace = (regionRectObj*)trace->getNext();
		}
	}
}
	

// Pass in a point and this will pass back if the point is contained in this regon or not.	
bool region::pointInRegion(point* inPoint) {

	regionRectObj*	trace;
	
	if (inPoint) {
		if (isEmpty()) {
			return false;
		}
		if (!enclosingRect.inRect(inPoint)) {
			return false;	
		}
		trace = (regionRectObj*)getFirst();
		while(trace) {
			if (trace->inRect(inPoint)) {
				return true;
			}
			trace = (regionRectObj*)trace->getNext();
		}
	}
	return false;
}


// Pass in a rect and this will pass back if the rect overlaps our region or not.
bool region::rectOverlapRegion(rect* inRect) {

	regionRectObj*	trace;
	
	if (inRect) {
		if (isEmpty()) {
			return false;
		}
		if (!enclosingRect.overlap(inRect)) {
			return false;	
		}
		trace = (regionRectObj*)getFirst();
		while(trace) {
			if (trace->overlap(inRect)) {
				return true;
			}
			trace = (regionRectObj*)trace->getNext();
		}
	}
	return false;
}
	
	
// Pass in a region and this will pass back if the two regions overlap or not.
bool region::regionOverlapRegion(region* inRegion) {

	regionRectObj*	traceA;
	regionRectObj*	traceB;
	rect				theirEnclosingRect;
	
	if (inRegion) {
		if (isEmpty()||inRegion->isEmpty()) {
			return false;
		}
		theirEnclosingRect = inRegion->getEnclosingRect();
		if (!enclosingRect.overlap(&theirEnclosingRect)) {
			return false;	
		}
		traceA = (regionRectObj*)getFirst();
		while(traceA) {
			traceB = (regionRectObj*)inRegion->getFirst();
			while(traceB) {
				if (traceA->overlap(traceB)) {
					return true;
				}
				traceB = (regionRectObj*)traceB->getNext();
			}
			traceA = (regionRectObj*)traceA->getNext();
		}
	}
	return false;
}


rect region::getEnclosingRect(void) { return enclosingRect; }


// Create a new regionRectObj using an inputted rect.
regionRectObj::regionRectObj(rect* inRect)
	: linkListObj(),
	rect(inRect) {  }
	
	
// Create a new regionRectObj using x,y width and height values.
regionRectObj::regionRectObj(int inX,int inY,int inWidth,int inHeight)
	: linkListObj(),
	rect(inX,inY,inWidth,inHeight) {  }
	
	
// Your standard destructor..
regionRectObj::~regionRectObj(void) {  }


