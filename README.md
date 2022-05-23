# SnowBrawl+
## An HTML & PC remake of Friezking's SnowBrawl on Scratch.
I made this in Grade 7, It is a remake of [SnowBrawl](https://scratch.mit.edu/projects/195065026/), Which is game made by one of my friends. Recently I ported this game to the web using emscripten.
## How to build
#### Windows PC
The original pc version of the game, I mainly build this with Msys2 and MinGW64
1. Download CMake from https://cmake.org/
1. Download and Install MSys2 [here](https://www.msys2.org/)
2. Inside the MSys2 terminal, install MinGW along with other tools using these commands
`pacman -Syu`
`pacman -S mingw-w64-x86_64-toolchain`
3. Open the Msys2 terminal in the games parent directory and run `.\win_build.bat`
    The game will now begin compiling. The copy will build to `build\bin`. Enjoy!

#### Web
This build uses emscripten to compile for the web.
Similar to Windows PC You'll need cmake, you'll also need make from MSys2.
1. Follow this [Tutorial](https://emscripten.org/docs/getting_started/downloads.html) to install Emscripten.
2. Download and Install Msys2 [here](https://www.msys2.org/) (if you followed the building windows PC tutorial, you can skip step 2 and 3)
3. Inside the MSys2 terminal Install make using these commands
`pacman -Syu`
`pacman -S make`
4. Open the Msys2 terminal in the games parent directory
3. Use the `export` command to set the EMSCRIPTEN environment variable to the toolchain directory
for example `F:\emsdk\upstream\emscripten`
5. run `.\web_build.bat`
   The game will now begin compiling. The copy will build to `build\bin`. 
6.(Extra Step) open the `build\bin` directory in the Msys2 terminal and run the game using the command
`emrun index.html` 

[![alt text](https://az743702.vo.msecnd.net/cdn/kofi3.png)](https://ko-fi.com/grimofender) 
[My Website](http://afkpals.net)