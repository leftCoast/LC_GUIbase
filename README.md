# LC_GUIbase
Foundation of the Left Coast GUI framework.

This is still a work in progress. Even so, everything I write is typically based o it. So what is here, works.  

**There are two main bases for the GUI base.**

**First is the displayObj class.** This is the common, base class interface to whatever hardware that one plans on using for their project. The purpose of diplayObj is to give the drawing environment a common interface regardless of hardware. Many will notice that it is much like the Adafruit GFX graphic library. And it is. This is because it was developed using their library as a model. The reason for developing a similar model, as opposed to just using theirs, was that I didn't want to be tied solely to the Adafruit product line. If I wanted to use some other hardware? No problem. I had my own interface ready to go.

The way diplayObj is used is to write a class that inherits from diplayObj and "glues" that to the driver code of the hardware. For Adafruit hardware this is nearly trivial because with few exceptions, its just pass throughs. As of this writing we have three Adafruit  part numbers : 1947, 1431 & 684. There is also an offscreen bitmap class that can be drawn to just like any other display for grabbing images. These images can be "blitted" to whatever hardware display is running on your system.

**All of this results in the global object, screen.** All drawing is accomplished through the global screen object.  

```
screen->fillScreen(&blue);
screen->drawRect(10,10,20,5,&green);
```


**The second is the drawObj class.** This is the base class of all items that can draw themselves on the screen. drawObj(s) can respond to clicks, if the hardware supports it. They can be moved, grouped resized. Any number of things. They "live" in a network of drawObj(s) that is managed behind the scenes during idle time.

In the LC_GUIbase library there is a global called viewList. Typically, in setup(), one creates their different drawObj(s) and adds them to viewList. Think of it like laying out playing cards. Bottom to top added to viewList. When a touch event occurs, it is passed from the top of viewList down, checking each drawObj until one of the drawObj(s) accepts the event. When not passing events down the viewList the program starts at the bottom and travels up the list seeing if any of the drawObj(s) need to be redrawn. In this way most of the GUI runs automatically. The only thing the programmer needs to concern herself with is devloping her custom drawObj(s) that accomplish what is needed for the application.
