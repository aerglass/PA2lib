<h1> PA2lib library </h1>
PA2lib is a library that aims to be similar to the now obsoleted PAlib, which fell against devkitARM r33 RIP.<br><br>

It can compile some PAlib code, not all but it has some compatibility, suporting for now bg and text functions.<br>
It is based on NFlib. Still I wouldn't recommend you to use this lib as your main lib because it can be painful (unexpected errors, compatibility issues, maybe even a nuclear explosion)

<h1> Usage </h1>
PA2lib is pretty simple, you need a custom Makefile, I made one but it does not support nitroFS for some reason, that means a ton of stuff will be broken so it is not my fault that you don't have the custom Makefile jaja. It must have some rules that are used in PAlib proyects to link graphics. I will be using Manjaro Linux for the process, but it will be similar to other linux distros or even windows or mac.<br><br>

Get yourself the latest release of PA2lib if you want the best stability, if you want to die of unexpected errors you can get the main branch too!.<br>
If you can, COMPILE THE LIB BY YOURSELF, if you can't then you are out of luck. Go to the PA2 folder, open a terminal type in "make clean" and delete the build and lib folders. after that type in "make" into the termianl. You are done compiling, easy right?<br><br>

Now lets integrate it into a PAlib proyect, get yourself the custom Makefile, I will problably upload it later, but keep in mind that it problably won't work under nitroFS/filesystem stuff. Delete the build and the Makefile, next you copy the custom Makefile that you created, create a folder called PA2 and copy the include and lib folders to the proyect's PA2 folder. try to compile after that, if it does not work then it is not my fault, wait 8 years until the support for that is added. PAlib is so complex that it will problably take more than 1 year to give PA2lib full support over PAlib code.<br>

<h1> Donating </h1>
Maintaining this takes so long but it is something i love to do.<br>
I only accept cryto but that may change in the future.<br>



Crypto Matic: 0x4032a81A79cB216C0F85a742Ee95D314176E8395<br>
Ethereum: 0x4032a81A79cB216C0F85a742Ee95D314176E8395<br>

