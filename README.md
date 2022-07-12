# Don't Eat My Sheep!

![screenshotte](screenshots/screenshot_a.png)

## Motivation

Someone wanted to c me write a program in C. So here's a puzzle game written in C99.

## How2play

The dragon wants to eat up the livestock so the knight wants to slay the dragon before that can
happen. Click on a piece, then click a square next to the piece to make a move. Pieces only move 1
square at a time and not diagonally. After a piece moves, all other pieces must move once before it
can move again.

In order to win, the knight must first be moved onto the same square as his horse in order to mount
up. Then he can slay the dragon by moving onto the same square as the dragon.

Press R to restart if you fail the puzzle. Press Escape to quit the app.

## Installation: Ubuntu

This app requires some packages installed:

    apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

May be used for static linking and it's possible this package will be different depending on your
computer's drivers or graphics card:
    
    apt-get install libgbm-dev

## Usage

Compile and run:

    ./start.sh

Run unit tests:

    ./start.sh test

## Credits

Terrain:
* Screaming Brain Studios: https://screamingbrainstudios.com/

Characters:
* Triberion and max.icons from Flaticon: https://www.flaticon.com/

Cooldown Icon:
* Roundicons from Flaticon: https://www.flaticon.com/

Font:
* GemFonts: http://moorstation.org/typoasis/designers/gemnew/home.htm

Music:
* Zane Little Music: https://zanelittle.com/
