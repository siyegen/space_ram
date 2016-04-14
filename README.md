SpaceRam
========

Ramparts inspired game, in SPACE(Sort of)!

Don't want to build and just want to play?!? Check the releases!
*[0.1.0](https://github.com/siyegen/space_ram/releases/tag/0.1.0)* | https://github.com/siyegen/space_ram/releases/tag/0.1.0

### How to Play
- Use S and W to go up/down and space to select
- Move the cursor with the mouse to aim at the 'evil' blocks and left click to destroy them
- ESC at any time to quit

### Features
- 3 exciting levels to choose from!
- Sounds!
- Movable camera and panning
- Color blocks just waiting to be destroyed!
- Tracks what levels you've completed so far (until you quit)

### Building
- Will only build as x86 not x64
- On windows you should just be able to open the project and build, libs are already linked and include paths are setup
- On os x you'll need to install glew and glfw3 (try homebrew)
- afterwards you can use the ./build.sh to output *game* and run that

### TODO
- Remove irrKlang and use openAL
- Make the enemies actually fight back, or at least move
- Online stats
- Player defined controls

### About
Uses OpenGL 3.2+, glew, glfw3, and glm

This is the result of 71 commits over 10 days straight of nights and weekends to finish a game in under 2 weeks.

This was a test to see what it actually took to get a playable (not necessarily fun) game finished. That meant 3d graphics, text hud sounds, and some form of notification that you had won. Reducing the scope so that I could finish it in the time given and not spend a lot of time writing useless layers of abstraction was challenging but very rewarding.

I stuck at low-level as I could considering my inexperience with c++ and graphics/game programming in general. I did end up using a higher level lib for images (**SOIL**) though I only used it ot write my own bitmap text renderer (which was fun). I also used irrKlang for the few sounds in the game as I keep having crashes when checking out openAL and was running up against my deadline.  

made with intense fury by @siyegen using VS2015 and Atom.io
