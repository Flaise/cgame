
# CONVERT PNG TO C

if [[ $1 != 'test' ]]; then
    mkdir -p ./src/res || exit 1
    xxd --include "./res/Tiny Top Down 32x32.png" ./src/res/terrain.h
    xxd --include "./res/dragon.png" ./src/res/dragon.h
    xxd --include "./res/knight.png" ./src/res/knight.h
    xxd --include "./res/knight_mounted.png" ./src/res/knight_mounted.h
    xxd --include "./res/sheep.png" ./src/res/sheep.h
    xxd --include "./res/dog.png" ./src/res/dog.h
    xxd --include "./res/horse.png" ./src/res/horse.h
    xxd --include "./res/hourglass2.png" ./src/res/cooldown.h
    xxd --include "./res/instructions.png" ./src/res/instructions.h
    xxd --include "./res/success.png" ./src/res/success.h
    xxd --include "./res/failure.png" ./src/res/failure.h
    xxd --include "./res/sinister_abode.ogg" ./src/res/music.h
fi


# COMPILE C

BIN_BASE="./bin/dont_eat_my_sheep"

if [[ $1 == 'test' ]]; then
    OPTS="-D DEBUG -D TEST -Og"
    BIN="${BIN_BASE}_test"
elif [[ $1 == 'release' ]]; then
    OPTS="-O3"
    BIN="${BIN_BASE}"
else
    OPTS="-D DEBUG -Og"
    BIN="${BIN_BASE}_debug"
fi

mkdir -p ./bin/ || exit 1

cd src || exit 1 # removes extraneous folder name from log messages
/usr/bin/time -f "compilation: %es" \
    c99 -Wall -o ../${BIN} *.c `pkg-config --cflags --libs sdl2 SDL2_image SDL2_mixer` ${OPTS} \
    || exit 1
cd .. || exit 1

# TODO: need --static flag when statically linking
# c99 -Wall -o ../${BIN} *.c `pkg-config --cflags --libs --static sdl2 SDL2_image` ${DEFINE} \

# EXECUTE

${BIN}
