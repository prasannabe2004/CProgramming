//
// breakout.c
//
// Computer Science 50
// Problem Set 4
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include "gevents.h"
#include "gobjects.h"
#include "gwindow.h"

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    double velocityOfX = 1.5;
    double velocityOfY = 1.5;
        
    waitForClick();
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        updateScoreboard(window, label, points);
        move(ball, velocityOfX, velocityOfY);
        pause(10);
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle)/2;
                double y = 550;
                setLocation(paddle, x, y);
            }
        }
        GObject object = detectCollision(window,ball);
        if (object != NULL) // Thanks GDB
        {
            if(object == paddle)
            {
                velocityOfY = -velocityOfY;
            }
            else if(strcmp(getType(object), "GRect") == 0)
            {
                bricks = bricks - 1;
                removeGWindow(window,object);
                velocityOfY = -velocityOfY;
                points = points + 1;
            }
        }
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityOfX = -velocityOfX;
        }
        if (getX(ball) <= 0 )
        {
            velocityOfX = -velocityOfX;
        }
        if (getY(ball) <= 0)
        {
            velocityOfY = -velocityOfY;
        }
        if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            lives--;
            setLocation(ball, 190, 200);
            setLocation(paddle, 160, 500);
            waitForClick();
        }
        if(points == 50)
        {
            updateScoreboard(window, label, points);
            break;
        }

    }
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int y = 50;
    int x = 0;
    int flag = 0;
    #define BRICK_WIDTH   30
    #define BRICK_HEIGHT  10
    for (int i = 0; i < ROWS; i++)
    {
        x = 0;
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(x , y, BRICK_WIDTH, BRICK_HEIGHT);            
            setFilled(brick, true);
            add(window, brick);
            x = x + 40; 
            if(flag)
            {
                setColor(brick, "YELLOW");
                flag = 0;
            }
            else
            {
                setColor(brick, "RED");
                flag = 1;
            }
        }
        y = y + 12;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // TODO
    // instantiate circle
    GOval circle = newGOval(200-RADIUS, 300-RADIUS, 2*RADIUS, 2*RADIUS);
    add(window, circle);
    setColor(circle, "RED");
    setFilled(circle, true);
    //setLocation(circle, 200,300);
    return circle;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    // TODO
    #define WIDTH_PADDLE 60
    #define HEIGHT_PADDLE 10
    GRect rectangle = newGRect(200-(WIDTH_PADDLE/2),550, WIDTH_PADDLE, HEIGHT_PADDLE);
    setColor(rectangle, "RED");
    setFilled(rectangle, true);
    add(window, rectangle);
    return rectangle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel(" ");
    add(window, label);
    setLocation(label, 200, 300);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
