/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{

    int k=1;
    for ( int i = 0; i < d ; i++)
    {
        for ( int j = 0; j < d; j++,k++ )
        {
            board[i][j] = d*d - k;
            printf("%d ",board[i][j]);
        }
    }
    if(d%2 == 0)
    {
        int temp =0;
        temp = board[d-1][d-2];
        board[d-1][d-2] = board[d-1][d-3];
        board[d-1][d-3] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for ( int i = 0; i<d;i++)
    {
        for (int j=0;j<d;j++)
        {
            if (board[i][j] != 0)
                printf("%2d ", board[i][j]);
            else
                printf("%2c ", '_');
        }
        printf("\n");
    }
    
}
void swapValues(int src_row,int src_col,int dst_row, int dst_col)
{
    int temp = 0 ;
    //printf("before swapping %d %d\n",board[src_row][src_col], board[dst_row][dst_col]);
    temp = board[src_row][src_col];
    board[src_row][src_col] = board[dst_row][dst_col];
    board[dst_row][dst_col] = temp;
    //printf("after swapping %d %d\n",board[src_row][src_col], board[dst_row][dst_col]);
}
/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // TODO
    int flag = 0, i, j, tile_row=0,tile_col=0;
    for (  i =0;i<d;i++)
    {
        for ( j=0;j<d;j++)
        {
            if(board[i][j] == tile)
            {
                //printf("tile found\n");
                flag = 1;
                tile_row = i;
                tile_col = j;
                break;
            }
        }
        if(flag)
            break;
    }
    //printf("Tile is at %d,%d\n",tile_row,tile_col);
    
    if (board[tile_row][tile_col+1] == 0 && tile_col+1 <=d-1 )
    {
        //printf("case1 swapping %d@%d,%d %d@%d,%d\n",board[tile_row][tile_col],tile_row,tile_col,board[tile_row][tile_col+1],tile_row,tile_col+1);
        swapValues(tile_row,tile_col,tile_row, tile_col+1);
        return true;
    }
    else if (board[tile_row][tile_col-1] == 0 && tile_col-1 >=0)
    {
        //printf("case2 swapping %d@%d,%d %d@%d,%d\n",board[tile_row][tile_col],tile_row,tile_col,board[tile_row][tile_col-1],tile_row,tile_col-1);
        swapValues(tile_row,tile_col,tile_row, tile_col-1);
        return true;
    }
    else if (board[tile_row+1][tile_col] == 0 && tile_row+1 <=(d-1))
    {
        //printf("case3 swapping %d@%d,%d %d@%d,%d\n",board[tile_row][tile_col],tile_row,tile_col,board[tile_row+1][tile_col],tile_row+1,tile_col);
        swapValues(tile_row,tile_col,tile_row+1, tile_col);
        return true;
    }
    else if (board[tile_row-1][tile_col] == 0 && tile_row-1 >=0)
    {
        //printf("case4 swapping %d@%d,%d %d@%d,%d\n",board[tile_row][tile_col],tile_row,tile_col,board[tile_row-1][tile_col],tile_row-1,tile_col);
        swapValues(tile_row,tile_col,tile_row-1, tile_col);
        return true;
    }
    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    // TODO
    int i = 0, j = 0,  k = 0;

    for (i =0 ;i<d;i++)
    {
        for (j=0;j<d;j++)
        {
            k++;
            //printf("tile = %d k=%d i=%d j=%d\n",board[i][j], k,i,j);
            if ( board[i][j] != k && k != (d*d))
            {
                return false;
            } 
        }
    }
    if (k == d*d && board[d-1][d-1] == 0)
    {
        return true;
    }
    else
    {
        printf("falling to default\n");
        return false;
    }
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
