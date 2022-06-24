
mkdir -p ./out/ || exit 1

cd src || exit 1 # removes extraneous folder name from log messages
c99 -o ../out/cgame *.c `pkg-config --cflags --libs sdl2` || exit 1
cd .. || exit 1

./out/cgame
