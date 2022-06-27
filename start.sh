
# CONVERT PNG TO C

mkdir -p ./src/res || exit 1
# xxd --include "./res/Floor - Grass 1 64x64.png" ./src/res/floor.h
# xxd --include "./res/Floor - Dirt 2 64x64.png" ./src/res/floor.h
# xxd --include "./res/Wall - Brick 1 64x64.png" ./src/res/wall.h
# xxd --include "./res/Wall - Brick 3 64x64.png" ./src/res/wall.h
xxd --include "./res/Tiny Top Down 32x32.png" ./src/res/terrain.h
xxd --include "./res/dragon.png" ./src/res/dragon.h
xxd --include "./res/knight.png" ./src/res/knight.h
xxd --include "./res/sheep.png" ./src/res/sheep.h


# COMPILE C

mkdir -p ./bin/ || exit 1

cd src || exit 1 # removes extraneous folder name from log messages
/usr/bin/time -f "compilation: %es" \
    c99 -o ../bin/cgame *.c `pkg-config --cflags --libs sdl2 SDL2_image` || exit 1
cd .. || exit 1


# EXECUTE

./bin/cgame
