
mkdir -p ./out/ || exit 1
c99 -o ./out/cgame src/main.c `pkg-config --cflags --libs sdl2` || exit 1

./out/cgame
