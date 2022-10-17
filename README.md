# CS408 Assignment 2
Nichoals Kelly

Introduction
First off I would just like to address that the repository is called CS408-Tutorial4 because the assignment was started using the tutorial as a base.

How to run
To run my project you will have to download the contents by clicking the green code button and downloading zip file.
![Screenshot (97)](https://user-images.githubusercontent.com/98287937/196088566-e2d3ec4b-ad9f-46ac-9a7a-d74c0d2d183c.png)

Once done that you will unzip that file and open your program environment and create a new C++ project. I personally use Visual Studio 2022. Once your new project is
created you will need to take the contents of the zip folder and place them in the project folder wherever that is on your computer. Make sure that everything stays in 
one folder or else it won't run. If you use visual studio 2022 you will need to go to solution explorer and add existing items, make sure to add all Header, c++, text files
and BMP files, do not add .obj files or the code will not compile. Once all is added I have the code set up to run animation1.txt.
![Screenshot (99)](https://user-images.githubusercontent.com/98287937/196089092-e9283015-1895-4203-9076-4f43a6837cf5.png)
In the readFile function, this can be changed to also run the example animation given to use by our professor I named it test.txt. Once the program is compiled for me
using visual studio 2022 I just hit the green play button, the animation should begin and after the last keyframe movement is performed the console will read animation
complete this will exit in 5 seconds. On the console screen there will be a printout of every frame and the values of every object during that frame unless the object
doesn't exist anymore then at the frame it stopped existing it will have a message.
