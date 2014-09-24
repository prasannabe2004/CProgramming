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

#define WIDTH_BRICK   30
#define HEIGHT_BRICK  8
#define WIDTH_PADDLE 60
#define HEIGHT_PADDLE 10
       
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
    double velocityOfY = drand48() + 1 ;
    
    //printf("velocity of y =%f\n",velocityOfY);
    waitForClick();
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        updateScoreboard(window, label, points);
        move(ball, velocityOfX, velocityOfY);
        
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
            setLocation(ball, 200-RADIUS, 300-RADIUS);
            setLocation(paddle, 200-(WIDTH_PADDLE/2), 550);
            waitForClick();
        }
        if(points == 50)
        {
            updateScoreboard(window, label, points);
            break;
        }
        pause(8);
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
    int k =0;
    
    for (int i = 0; i < ROWS; i++)
    {
        x = 2;
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(x , y, WIDTH_BRICK, HEIGHT_BRICK);            
            setFilled(brick, true);
            add(window, brick);
            x = x + 40;
            switch (ROWS-k)
            {
            /* 
             *    BLACK, BLUE, CYAN, DARK_GRAY, GRAY, GREEN, LIGHT_GRAY, MAGENTA, ORANGE, PINK, RED, WHITE, YELLOW
             */
                case 1:
                    setColor(brick, "CYAN");
                    break;
                case 2:
                    setColor(brick, "GREEN");
                    break;
                case 3:
                    setColor(brick, "ORANGE");
                    break;
                case 4:
                    setColor(brick, "YELLOW");
                    break;
                case 5:
                    setColor(brick, "RED");
                    break;                                        
            }
        }
        k++;
        y = y + 12;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    GOval circle = newGOval(200-RADIUS, 300-RADIUS, 2*RADIUS, 2*RADIUS);
    if (circle)
    {
        add(window, circle);
        setColor(circle, "BLACK");
        setFilled(circle, true);
        //setLocation(circle, 200,300);
        return circle;
    }
    else
    {
        return NULL;
    }
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect rectangle = newGRect(200-(WIDTH_PADDLE/2),550, WIDTH_PADDLE, HEIGHT_PADDLE);
    if(rectangle)
    {
        setColor(rectangle, "GRAY");
        setFilled(rectangle, true);
        add(window, rectangle);
        return rectangle;
    }
    else
    {
        return NULL;
    }
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel(" ");
    if(label)
    {
        setFont(label, "SansSerif-40");
        setColor(label,"LIGHT_GRAY");
        add(window, label);
        setLocation(label, 200, 300);
        updateScoreboard(window, label, 0);
        return label;
    }
    else
    {
        return NULL;
    }
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
