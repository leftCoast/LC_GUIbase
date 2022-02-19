#include <baseGraphics.h>
#include <math.h>

#define DEF_LOC_X  16    // Just anything so the user can see something.
#define DEF_LOC_Y  16
#define DEF_SIZE_X 16
#define DEF_SIZE_Y 16

/*
void printPoint(point* inPt,char* name) {

	Serial.print(name);
	Serial.print(inPt->x);
	Serial.print(F(", "));
	Serial.println(inPt->y);
}
*/
	
int	xDistance(point ptA,point ptB) { return ptB.x - ptA.x; }


int	yDistance(point ptA,point ptB) { return ptB.y - ptA.y; }


float distance(point ptA,point ptB) {
	
	float xDist;
	float yDist;
	float widthSq;
	float heighSq;
	
	xDist = xDistance(ptA,ptB);
	yDist = yDistance(ptA,ptB);
	widthSq = xDist * xDist;
	heighSq = yDist * yDist;
	return sqrt(widthSq + heighSq);
}


float	angle(point ptA,point ptB) {

	float xDist;
	float yDist;
	float	angle;
		
	xDist = xDistance(ptA,ptB);
	yDist	= yDistance(ptA,ptB);
	if (xDist) {
		angle = atan(yDist/xDist);
	} else {
		angle = M_PI/2;
	}
	return angle;
}


// Rotate around the x.y axis. From current x,y location, angle (radians).
// BUT this is only using ints so the accuracy is somewhat lacking.
point	rotate(point ptA,float angle) {

	point	res;
	float	mag;
	
	mag = sqrt((ptA.x * ptA.x) + (ptA.y * ptA.y));
	res.x = mag * cos(acos(ptA.x/mag)+angle);
	res.y = mag * sin(asin(ptA.y/mag)+angle);
	return res;
}


/*
// HANDY for tracing issues through the drawing code.
void rect::printRect(char* label) {

	Serial.print(label);
	Serial.print(F("x:"));Serial.print(x);Serial.print(F(" "));
	Serial.print(F("y:"));Serial.print(y);Serial.print(F(" "));
	Serial.print(F("w:"));Serial.print(width);Serial.print(F(" "));
	Serial.print(F("h:"));Serial.println(height);
}
*/


rect::rect(void) {
   
   setLocation(DEF_LOC_X,DEF_LOC_Y);
   setSize(DEF_SIZE_X,DEF_SIZE_Y);
 }
 
 
rect::rect(rect* inRect) { setRect(inRect); }


rect::rect(int inX, int inY, int inWidth,int inHeight) {
   
   setRect(inX,inY,inWidth,inHeight);
}

 
rect::~rect(void) { }
 
 
void  rect::setLocation(int inX, int inY) {
  
  x = inX;
  y = inY;
}


void  rect::setSize(int inWidth,int inHeight) {

  width = inWidth;
  height = inHeight;
}


void rect::setRect(rect* inRect) {
  
  setLocation(inRect->x,inRect->y);
  setSize(inRect->width,inRect->height);
}


void rect::setRect(point* inPt1,point* inPt2) {

  int locX = min(inPt1->x,inPt2->x);
  int locY = max(inPt1->y,inPt2->y);
  int width = abs((int)inPt1->x-(int)inPt2->x);
  int height = abs((int)inPt1->y-(int)inPt2->y);
  setLocation(locX,locY);
  setSize(width,height);
}


void rect::setRect(int inX, int inY, int inWidth,int inHeight) {

	setLocation(inX,inY);
   setSize(inWidth,inHeight);
}

// Become the enclosing rect of yourself and this rect.
void  rect::addRect(rect* inRect) {

	rect	result(this);
	
	if (inRect)	{													// If we got a rect..
		result.x			= min(x,inRect->x);					// X would be the minimum of ours && inRect's x values.
		result.y			= min(y,inRect->y);					// Y would be the minimum of ours && inRect's y values.
		if (maxX()>inRect->maxX()) {							// If our max x is larger that inRect's max x..
			result.width = maxX() - result.x;				// Then width would be Our max x minus result's x value.
		} else {														// Else, if our max x is NOT larger than BinRect's max x..
			result.width = inRect->maxX() - result.x;		// Then width would be inRect's max x minus the resulting x value.
		}
		if (maxY()>inRect->maxY()) {							// If our max y is larger that inRect's max y..
			result.height = maxY() - result.y;				// Then height would be our max y minus result's y value.
		} else {														// Else, if our max Y is NOT larger than inRect's max Y..
			result.height = inRect->maxY() - result.y;	// Then width would be inRect's max y minus the resulting y value.
		}
	}
	setRect(&result);
}
	
	
	
	
void rect::insetRect(int inset) {

	x = x + inset;
	y = y + inset;
	width = width - (2*inset);
	height = height - (2*inset);
}
          	
          	
int rect::maxX(void) { return(x + width); }
int rect::maxY(void)  { return(y + height); }
int rect::minX(void) { return(x); }
int rect::minY(void)  { return(y); }


bool rect::inRect(int inX, int inY) {

  return(
  inX >= minX() &&
  inX <= maxX() &&
  inY >= minY() &&
  inY <= maxY()
  );
}


bool rect::inRect(point* inPoint) { return inRect(inPoint->x,inPoint->y); }


point rect::getCorner(rectPt corner) {

	point	tempPt;
	
	switch(corner) {
		case topLeftPt :
			tempPt.x = x;
			tempPt.y = y;
		break;
		case topRightPt :
			tempPt.x = x + width;
			tempPt.y = y;
		break;
		case bottomLeftPt :
			tempPt.x = x;
			tempPt.y = y + height;
		break;
		case bottomRightPt :
			tempPt.x = x + width;
			tempPt.y = y + height;
		break;
	}
	return tempPt;
}


// Are we touching this passed in rectangle?
bool rect::overlap(rect* checkRect) {

		if(maxX()<checkRect->minX()) return false;
		if(minX()>checkRect->maxX()) return false;
		if(maxY()<checkRect->minY()) return false;
		if(minY()>checkRect->maxY()) return false;
		return true;
	}


// Are we contained in this passed in rectangle?
bool rect::isSubRectOf(rect* checkRect) {
	
	point tr;
	point tl;
	point br;
	point bl;
	
	tr = getCorner(topRightPt);
	tl = getCorner(topLeftPt);
	br = getCorner(bottomRightPt);
	bl = getCorner(bottomLeftPt);
	
	return checkRect->inRect(&tr)
			&& checkRect->inRect(&tl)
			&& checkRect->inRect(&br)
			&& checkRect->inRect(&bl);
}







