# Arkanoid-C
Arkanoid clone written in C

LINUX/MAC: works without any extra requirement

Just run the makefile as normal and should work
$make

For developer tools use $make DEV=1
This enables the use of 'l' and 'L' for skipping levels

Some features we thought you should know:

Brick color: each color has a different base value for your score:

- RED 50
- GREEN 60
- YELLOW 70
- BLUE 80
- MAGENTA 90
- CYAN 100
- WHITE: Has 2 health and adds 50 points to the score. Every 8 levels the score that it gives increases by an extra 50 points.

Capsule color:
- ENLARGE - RED
increases paddle size for 10 seconds.

- SLOW - GREEN
halves ball speed for 5 seconds.

- BREAK - YELLOW
pass to the next level while getting the score for breaking all the bricks on that level.

- DISRUPTION - BLUE
split ball into 3 playable balls. 

- LIFE - MAGENTA
grants one extra life. 

There is a speed mult, so you will gain more points as the game progresses. 
Speed increases every 4 levels. 

If you pass a level with extra balls, each one adds an additional score equal to the score for that level to your total. 
Survive with 2 balls: that's 2x points for that level
Survive with 3: THAT'S 3X!

Have fun!
