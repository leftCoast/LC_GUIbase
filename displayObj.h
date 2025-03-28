#ifndef displayObj_h
#define displayObj_h

#include <Arduino.h>
#include <gfxfont.h>
#include <colorObj.h>
#include <baseGraphics.h>
#include <bitmap.h>


// This is our base display object. Similar to adafruit's GFX object.
// The plan is to inherit this for whatever glue code each piece of 
// hardware needs to run.


// Rotation
#define PORTRAIT      0  // Default narrow.
#define LANDSCAPE     1  // Default wide, clockwise.
#define INV_PORTRAIT  2  // Upside down narrow, clockwise.
#define INV_LANDSCAPE 3  // Upside down wide, clockwise.



class displayObj {    

	public :
				displayObj(bool inGraphic,bool inColor,bool inTouch,bool inSD,bool inReadable);
	virtual	~displayObj(void);

				void	pushOffset(int x, int y);
				void	popOffset(int x, int y);
				int	gX(int lX);
				int	gY(int lY);
				int	lX(int gX);
				int	lY(int gY);
				point	lP(point* gP);
				point	gP(point* lP);
				
	virtual	int	width(void);
	virtual	int	height(void);
	virtual	bool	begin(void);
  	virtual	void	startWrite(void);
  	virtual	void	endWrite(void);
	virtual	void	setRotation(byte inRotation);
	virtual	void	setTextColor(colorObj* tColor);
	virtual	void	setTextColor(colorObj* tColor,colorObj* bColor);
	virtual	void	setTextSize(byte inSize);
	virtual	void	setTextWrap(bool inWrap);
	virtual	void	setFont(const GFXfont* font);
	virtual	void	setCursor(int inX,int inY);
	virtual	int	getCursorX(void);
	virtual	int	getCursorY(void);
	virtual	rect	getTextRect(const char* inText);
	virtual	void	drawText(const char* inText);
	virtual	void  drawChar(int x,int y,char inChar,colorObj* fColor,colorObj* bColor,int size);
	virtual	void	fillScreen(colorObj* inColor);
	virtual	void	fillRect(int locX,int locY,int width,int height, colorObj* inColor);
	virtual	void	fillRect(rect* inRect,colorObj* inColor);
	virtual	void	drawRect(rect* inRect,colorObj* inColor);
	virtual	void	drawRect(int locX,int locY,int width,int height, colorObj* inColor);
	virtual	void	fillRoundRect(int locX,int locY,int width,int height,int radius,colorObj* inColor);
	virtual	void	drawRoundRect(int locX,int locY,int width,int height,int radius,colorObj* inColor);
	virtual	void	fillRoundRect(rect* inRect,int radius,colorObj* inColor);
	virtual	void	drawRoundRect(rect* inRect,int radius,colorObj* inColor);
	virtual	void	drawCircle(int locX,int locY,int inDiam, colorObj* inColor);
	virtual	void	drawCircleHelper(int locX,int locY,int inRad,byte corner,colorObj* inColor);
	virtual	void	fillCircle(int locX,int locY,int inDiam, colorObj* inColor);
	virtual	void	drawTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor);
	virtual	void	fillTriangle(point* pt0,point* pt1,point* pt2,colorObj* inColor);
	virtual	void	drawVLine(int locX,int locY,int height,colorObj* inColor);
	virtual	void	drawHLine(int locX,int locY,int width,colorObj* inColor);
	virtual	void	drawLine(int locX,int locY,int locX2,int locY2,colorObj* inColor);
	virtual	void	drawLine(point* startPt,point* endPt,colorObj* inColor);
	virtual	void	drawPixel(int locX,int locY,colorObj* pColor);
   virtual	void	blit(int locX,int locY,bitmap* inBitmap);
   
	virtual	void	drawPixelInvert(int x,int y);
	virtual	void	frameRectInvert(int x,int y,int width,int height);
	virtual	void	fillRectGradient(int inX,int inY,int width,int height,colorObj* startColor,colorObj* endColor,bool rising=true,bool vertical=true);
	virtual	void	fillRectGradient(rect* inRect,colorObj* startColor,colorObj* endColor,bool rising=true,bool vertical=true);
	virtual	void	fillScreenGradient(colorObj* startColor,colorObj* endColor,bool rising=true,bool vertical=true);
	
	virtual	point	getPoint(void);
	virtual	bool	touched(void);
  
  				bool isGraphic(void);
  				bool isColor(void);
  				bool hasTouchScreen(void);
  				bool hadSDDrive(void);
  				bool canRead(void);
  
  protected:
  				int			offsetX;
  				int			offsetY;
				bool			graphic;
				bool			color;
				bool			hasTouch;
				bool			hasSD;
				bool			readable;
				bool			boundsBegin;
				bool			boundsRecording;
};


extern displayObj* screen;

#endif

