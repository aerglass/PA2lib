# The (unstable) PA Compatibility layer bundled with PA2lib

The PA Compatibility layer allows for PA2lib (and possibly nightfox's lib with some work) to compile (extremelly simple) PAlib code.<br>
Currently only text functions are supported (PA_InitText, PA_OutputText, PA_OutputSimpleText) without any numbers included.</p>
<strong> THE PA COMPATIBILITY LAYER NEEDS A DEFAULT FONT (default.pal, default.fnt) INSIDE IN THE NITROFILES FOLDER.</strong></p>
Installation:<br>
Get the latest palib-backwards-compatibility branch master version of PA2lib, which already comes included with the PA Compatibility layer.<br>
<strong> YOU WANT TO COMPILE THE LIB BY YOURSELF DOUBLE-CLICK THE BUILD.BAT OR ELSE IT WON'T WORK.</strong></p>

Usage:<br>
Enter the folder on the PAlib proyect<br>
Copy the nitrofiles folder with a font named default.fnt and default.pal.<br>
Create the PA2 folder and copy the "include" and "lib" folders to the PA2 folder of the compile PA2 folder.<br>
Copy the makefile from one of the examples, it will work <br>
If you can, change the line <code> #include &lt;PA9.h&gt;</code> to <code>#include &lt;spa.h&gt;</code>
Now you want to run make on the root of the folder, Good luck man trying to make it actually work, it may need changes.<br>
