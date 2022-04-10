# PA2lib (Future: EUFLlib)
# im working on a entire rewrite of the lib in order to garantee performance, also to get rid of the creative commons license
Programmer's arsenal 2 (continuation of PALib and based on Nightfox's lib)</p>

a very small, simple but powerful library<br>
features:<br>
Very easy to learn <br>
is less than a megabyte <br>
text<br>
sprites<br>
backgrounds<br>
3D Sprites <br>
Functions for time delay or anything else <br>
Sounds (not music, use MaxMod for that)</p>
bad things:<br>
You NEED to use hbmenu to launch your game via r4!!!<br>
some R4 cards are not compatible due to nitrofs support<br>
The library is in public beta prerelease, it is not stable<br>
if you want to migrate from older versions/older palib, installation is somewhat complex. <br>
you need to convert your sprites using grit.<br>
May break on new libnds/devkitarm releases.<br>
<h1> Known Bugs </h1></p>
Somehow some images have random colors or descoloration. <br>
While i was making some examples the background made random stuff, so don't use gimp lol </p>
Workarrounds:<br>
fill the entire 256 colors with black, or else it wont work. or put -gB8 flag while converting the backgrounds.
<h2> why?</h2>
Nightfox's lib for libnds isn't updated anymore.
thus it can be incompatible with recent versions of libnds/devkitarm
  <h2> Why is called PA2lib </h2>
  PAlib is old, it was last updated on 2008, and it doesn't work with the latest version of
  libnds/devkitarm. Nightfox's lib is difficult but once you get it, you will be an expert of nintendo ds programming.
  So i decided to make nightfox's lib way easier
  
  <h2>nice, but how do i install it?</h2>
  Use the template, <strong>You can't install it. </strong> </p>
  If you really want to install it then compile the library.<br>
  copy the "lib" and "include" folders to devkitpro/libnds.<br>
  
<h2>Examples?</h2>
They are work in progress.<br>
You can contribute to make the examples, however, im working on it.</p>
<h2>Credits</h2>
<strong>thanks to knightfox 75 for nightfox's lib for libnds</strong><br>
aerglass because...</p>
<h2>how to compile? </h2>
download devkitpro (www.devkitpro.org/) </p>
next download the repository </p>
Here are 2 options, compile using the batch file, or with the cmd. <br>
double click the makelib.bat file and let it do its work, if it doesn't compile
then report it on github. <br>
Using the cmd (terminal) go to the directory that is PA2LIB is located, enter to the
pa2 folder, then type in "make clean" when it finishes, type "make" it will compile the lib.
<h1>Docs?</h1>
github wiki exist, i will be adding things into it.</p>
<h1> About updates </h1>
this updates when i have time, every change made into something that ins't on the releases tab is not stable.
<h1> how to update? </h1><br>
  delete the existing PA2 folder from you proyect. Then download the latest stable release (you can use the unstabe builds but they are unstable lol) and then drag the pa2 folder from the template, now you are done<br>
  <h1> Contact </h1></p>
  AerGlass#3242 on discord</p>
  <h1> Donate </h1></p>
  It takes long time and a lot of effort to get this working. (Maybe because i am the only dev maintaining trash) </p>
  
  Crypto:<br>
  Ethereum: 0x4032a81A79cB216C0F85a742Ee95D314176E8395<br>
  Polygon: 0x4032a81A79cB216C0F85a742Ee95D314176E8395</p>
  
  I don't think that anybody would want to donate to this trash but if you want then you can. </p>
<h1> license </h1> </p>
creative commons. But if you break the rules then i don't care, i just putted that licence due to nflib's license.
