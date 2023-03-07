### Required: sdl2, sdl2_image, sdl2_ttf, sdl2_mixer, sdl2_net, 
### cmake, make and pkgconfig

### Compilation on Windows ###

cd Baedim
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cd ..
mingw32-make -C build

### Run ###

./Baedim.exe

##################

### Compilation on Linux (Arch tested) ###
cd Baedim
mkdir build
cd build
cmake ..
cd ..
make -C build

### Run ###

./Baedim
