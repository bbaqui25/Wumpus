/* ----------------------------------------------
 * Basic Calculator with + - / % *
 *
 * To Do: Create a Hunt the Wumpus game
 * ----------------------------------------------
 */

#include <stdio.h>
#include <ctype.h>         // For converting lowercase letters to uppercase.
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        
#include <time.h>

//Global constants
static int arrows;        	//declaring arrows constant
static int debug = 0;	 	// declaring constant to debug errors in code

const int player = 0;		// This variable defines the room you are in
const int wumpus = 1;		// This variable defines the room the Wumpus is in
const int pit1 = 2;			// This variable defines the room for the first pit
const int pit2 = 3;			// This variable defines the room for the second pit
const int bat1 = 4;			// This variable defines the room for the first bat
const int bat2	= 5;		// This variable defines the room for the second bat
const int arrow = 6;		// This variable defines the room for the extra arrow
const int rooms	= 7;		// This variable defines the total rooms that are occupied at any point of time

static int location[rooms];	//Creating array for locationations of each room
static int finished;		// indicates that the game is finished by either winning or losing

static int cave[20][3] =	// creates array of 20 rooms and 3 tunnels for each room
{
    {1,4,7},
    {0,2,9},
    {1,3,11},
    {2,4,13},
    {0,3,5},
    {4,6,14},
    {5,7,16},
    {0,6,8},
    {7,9,17},
    {1,8,10},
    {9,11,18},
    {2,10,12},
    {11,13,19},
    {3,12,14},
    {5,13,15},
    {14,16,19},
    {6,15,17},
    {8,16,18},
    {10,17,19},
    {12,15,18},
};


//displayInfo displays information about author and program
//displayInfo also displays the instructions of the game
//the function does not return anything
void displayInfo()
{
	printf ("Author: Bushra Baqui                _______18______");
	printf ("\nCS 141 Program #6: Wumpus          /        |      \\"); 
	printf ("\nTA: Chenwei Zhang, T 9:00 AM      /       __9__     \\");
	printf ("\nApril 21, 2016                   /       /     \\     \\");
	printf ("\nSystem: Windows 10, DevC++      /       /       \\     \\");
    printf ("\n                       		17     8        10    19"); 
    printf ("\n                 	        | \\   / \\      / \\   / |");
	printf ("\nHunt the Wumpus:                |  \\ /   \\    /   \\ /  |");
	printf ("\nThe Wumpus lives in a           |   7     1---2    11  |");			
	printf ("\ncompletely dark cave of     	|   |    /     \\   |   |");
	printf ("\n20 rooms.  Each room has        |   6---5      3---12  |");
	printf ("\n3 tunnels leading to other      |   |     \\   /     |  |"); 
	printf ("\nrooms as shown:                 |    \\      4      /   |");
    printf ("\n                                |     \\     |     /    |");
    printf ("\n                                \\     15---14---13     /");
    printf ("\n                                 \\    /           \\   /");
    printf ("\n                                  \\  /             \\ /");
    printf ("\n                                   16--------------20");
	printf ("\nHazards:\n"
			"1. Two rooms have bottomless pits in them.  If you go there you fall and die.\n"
			"2. Two other rooms have super-bats.  If you go there, a bats grab you and\n"
   			"fly you to some random room, which could be troublesome.  Then those bats\n"
   			"go to some random room.\n"
			"3. The Wumpus is not bothered by the pits or bats, as he has sucker feet and\n"
   			"is too heavy for a bat to lift.  Usually he is asleep.  Two things wake\n"
  			"him up: Anytime you shoot an arrow, or you entering his room.  When he\n" 
   			"wakes he moves one room 75 percent of the time, staying still the other 25 percent of\n"   
   			"the time. After that, if he is in your room, he eats you and you die!\n\n"
   			"Moves:\n"	
			"On each move you can:\n"
			"1. Move into an adjacent room.  To move enter 'M' followed by a space and\n"
   			"then a room number.\n"  
			"2. Shoot your guided arrow through a list of four adjacent rooms, which\n" 
   			"you specify.  Your arrow ends up in the final room. If an arrow\n" 
   			"can't go a direction because there is no connecting tunnel, it ricochets\n"
  			"and moves to a randomly selected adjacent room and continues\n"
   			"doing this until it has traveled 4 rooms in total.  If the arrow\n" 
   			"hits the Wumpus, you win! If the arrow hits you, you lose. You\n" 
   			"automatically pick up an arrow if you go through a room with an arrow in\n" 
   			"it. To shoot enter 'S' followed by a list of up to 4 adjacent room numbers\n"
   			"separated by spaces.\n"
			"3. Undo your last move by entering 'U'\n\n"
			"Good luck!\n");
}//end of displayInfo()

int getNum(char* prompt) 
{
    int n;

    printf("%s: ", prompt);

    scanf("%d", &n);

    return n;
}//end of int getNum(char* prompt) 

// getChar function accepts user input of m (move), s (shoot) or c (cheat codes)
// if the user inputs n, the program exits. This is utilized for the final continue game? Y/N functionality
int getChar(char* prompt) 
{
    char c = '\n';	//initialize character value

    printf("%s ", prompt);
    
	while (c == '\n') 
	{
        scanf("%c", &c);
        
        if (c == 'c') 
		{
        	printf ("cheat\n");
			printf("You are at %d, Wumpus is at %d, pits at %d & %d, bats at %d & %d, Arrow is located at %d\n", 
					location[player]+1,location[wumpus]+1, location[pit1]+1, location[pit2]+1, location[bat1]+1, location[bat2]+1,location[arrow]+1);
    	}
    	
    	if (c == 'N')
    	{
    		printf ("Exiting...\n");
			exit(0);
		}
    }

    return toupper(c);
}//end of int getChar(char* prompt) 

//showRoom function prints what room you are in and what rooms the tunnels connect from your room
//showRoom also checks if the room you are in is adjacent to a room with wumpus, bats or pits and alerts you with a message
void showRoom() 
{
    printf("\n");
		
	printf("YOU ARE IN ROOM %d. TUNNELS LEAD TO %d %d %d\n",
				location[player]+1, cave[location[player]][0]+1, cave[location[player]][1]+1, cave[location[player]][2]+1 );
	
	
	for (int k = 0; k < 3; k++) 
	{
		int room = cave[location[player]][k];

		if (room == location[wumpus]) 
		{
	       
		   printf("There is:  a smell\n");
		} 
		else if (room == location[pit1] || room == location[pit2]) 
		{
	       
		   printf("There is:  a draft\n");
		} 
		else if (room == location[bat1] || room == location[bat2]) 
		{
	       printf("There is: a flapping sound\n");
    	}
    }
}//end of void showRoom() 
	
	
//moveOrShoot accepts m or s as the input from the user to determine if the user wants to move or shoot an arrow
int moveOrShoot(int &gameInProgress, int &win, int &lose) 
{
	char c;
	
	while ((c != 'S') && (c != 'M')) 
	{
        c = getChar((char *)"Your move: ");
    }

    return (c == 'S') ? 1 : 0;
}//end of int moveOrShoot() 


//Anytime the player shoots an arrow, or he enters the wumpus room, the wumpus wakes up
// When the wumpus wakes, he moves one room 75% of the time, staying still the other 25% of the time.
// After that, if wumpus is in the player's room, he eats the player and the player dies!
void moveWumpus(int &gameInProgress, int &win, int &lose) 
{
    int k = rand() % 4;

    if (k < 3) 
	{
       location[wumpus] = cave[location[wumpus]][k];
    }

    if (location[wumpus] == location[player]) 
	{
        printf("wumpus GOT player!\n");
        
		finished = lose;
    }
}//end of void moveWumpus() 


// shoot function is initated when the player chooses to shoot an arrow with the "s" command
// after the player has chosen to shoot an arrow, the shoot function prompts  
void shoot(int &gameInProgress, int &win, int &lose) 
{
    int path[3]; //declaring an array of 3 rooms that the player can choose to shoot an arrow thru
    int identifyRoom = -1;

    finished = gameInProgress;

	int len = 3;
    
    int k = 0;
	while (k < len) 
	{
        path[k] = getNum((char *)"ROOM #") - 1;

        if (((k>1) && (path[k] == path[k-2])) || ((k>1) && (path[k] == path[k-1])) || ((k>0) && (path[k] == path[k-1])))
		{
            printf("Please select distinct rooms to shoot the arrow. Re-enter the last room number \n");
            
			continue; 
    	} 

    	k++;
    }
 
    identifyRoom = location[player];

	// for loop checks if the user entered rooms to shoot the arrow through are connected by tunnels or not
    for (int k = 0; k < 1; k++) 
	{	
		// these if conditions check if the arrow has clear tunnels to go through in the user entered rooms
	    if ((cave[identifyRoom][0] == path[k]) || (cave[identifyRoom][1] == path[k]) ||
            (cave[identifyRoom][2] == path[k])) 
		{
			if ((cave[path[k]][0] == path[k+1]) || (cave[path[k]][1] == path[k+1]) ||
            (cave[path[k]][2] == path[k+1])) 
			{
				if ((cave[path[k+1]][0] == path[k+2]) || (cave[path[k+1]][1] == path[k+2]) ||
            	(cave[path[k+1]][2] == path[k+2])) 
				{
					identifyRoom = path[k+2];
				}
			}
		} 
        
        // if the rooms provided by the player does not show a clear path for the arrow, the arrow ricochets and
		// moves to a randomly selected adjacent room and continues doing this until it has traveled 3 rooms in total
		else 
		{
            identifyRoom = cave[identifyRoom][rand()%3];
            identifyRoom = cave[identifyRoom][rand()%3];
            identifyRoom = cave[identifyRoom][rand()%3];
            identifyRoom = cave[identifyRoom][rand()%3];
        }

        // if arrow hits the wumpus, then player gets a win message
		if (identifyRoom == location[wumpus]) 
		{
			printf("AHA! player GOT THE wumpus!\n");
            
			finished = win;
        } 
        
        // if arrow hits the player, the player loses
		else if (identifyRoom == location[player]) 
		{
            printf("OUCH! arrow GOT player!\n");
        
			finished = lose;
        }

        // if arrow misses the player and the wumpus, the game continues
		if (finished != gameInProgress) 
		{
		    return;
        }
    }

	printf("Arrow Missed its target\n");
	
	// moveWumpus function moves the wumpus once it wakes up. The wumpus wakes when an arrow is shot.
	moveWumpus(gameInProgress, win, lose);

	// if condition to check if there are any more arrows left. If no more arrows, game over
	if (--arrows <= 0) 
	{
	    finished = lose;
    }
}//end of void shoot()


// if the player selects to move to another room
void move(int &gameInProgress, int &win, int &lose) 
{
    int identifyRoom = -1;

    //while loop gets room input fro user and checks if the room entered by the player to move to is a valid and adjacent room
	while (identifyRoom == -1) 
	{
		identifyRoom = getNum((char *)"WHERE TO") - 1;
		
		// this if condition checks if the room entered by user is a valid room between 1 to 20
        if (identifyRoom < 0 || identifyRoom > 19) 
		{
            identifyRoom = -1;
            printf("Please enter a valid move between 1 to 20\n");
            continue;
        }
		
		//this if condiion checks if the room entered by player to move to is an adjacent room
        if ((cave[location[player]][0] != identifyRoom) && (cave[location[player]][1] != identifyRoom) &&
			(cave[location[player]][2] != identifyRoom) && (location[player] != identifyRoom)) 
		{
		    printf("That move is invalid.  Please retry\n");

            identifyRoom = -1;
            
			continue;
        }
    }
   
    location[player] = identifyRoom;
	
	// checks if the room the player has entered the bat1 room. 
	// If yes, the player is dropped in a random room and the bat1 relocationate to another random room
    while (identifyRoom == location[bat1])
	{
        printf("ZAP--SUPER BAT SNATCH! ELSEWHEREVILLE FOR player!\n");
        
		identifyRoom = location[player] = rand()%20;
		location[bat1]= rand()%20;
		
		do 
		{
			location[bat1]= rand()%20;
		}while (location[player]=location[bat1]);	
    }
	
	// checks if the room the player has entered the bat2 room. 
	// If yes, the player is dropped in a random room and the bat2 relocationate to another random room
	while (identifyRoom == location[bat2]) 
	{
        printf("ZAP--SUPER BAT SNATCH! ELSEWHEREVILLE FOR player!\n");
        
		identifyRoom = location[player] = rand()%20;
		location[bat2]= rand()%20;
		do 
		{
			location[bat2]= rand()%20;
		}while (location[player]=location[bat2]);	
    }

	// checks if the room the player has entered is the whumpus room. If yes the player loses and the wumpus moves
    if (identifyRoom == location[wumpus]) 
	{
	   printf("... OOPS! BUMPED A wumpus!\n");
	   
	   moveWumpus(gameInProgress, win, lose);
    } 
	
	//checks if the room the player has entered is pit1 or pit2. If yes, the player loses.
    if (identifyRoom == location[pit1] || identifyRoom == location[pit2]) 
	{
	   printf("YYYYIIIIEEEE . . . FELL IN PIT\n");
	   
	   finished = lose;
    }
    
	// checks if the room the player has entered has arrow. If yes, the player gains an arrow
	// and arrow room is changed to a random room
    if (identifyRoom == location[arrow]) 
	{
	   printf("You Picked up an Additional Arrow!\n");
	   identifyRoom = location[arrow] = rand()%20;
	   ++arrows;   
    }
    
}//end of void move()


// gameSetup function assigns a room to you, wumpus, pit1, pit2,bat1 and bat2 
//It ensures you are never in the same room as any of them.
void gameSetup() 					
{
	for (int j = 0; j < rooms; j++) 
	{
		location[j] = -1;
        
		while (location[j] < 0) 
		{
			location[j] = rand()%20;

            for (int k=0; k<j-1; k++) 
			{
                if (location[j] == location[k]) 
				{
					location[j] = -1;
                }
        	}
    	}
    }
}//end of void gameSetup()


// gamePlay function calls out other functions to ensure the game can be played end to end
void gamePlay(int &gameInProgress, int &win, int &lose) 
{
	//initializing the game by providing the player with 5 arrows
	arrows = 5;  	
	
	// initailizing the "Game In Progress" mode 
    finished = gameInProgress;	
    
    // while the player has not won or lost the game, allows the player to move or shoot
	while (finished == gameInProgress) 
	{
	    showRoom();			// calling showroom function
	    
	    // if else condition directs the program to progress based on the user input to move or shoot
		if (moveOrShoot(gameInProgress, win, lose)) 
		{
            shoot(gameInProgress, win, lose);	//calling shoot function if player chooses to shoot arrow
        } 
		else 
		{
            move(gameInProgress, win, lose);	//calling move function if the player chooses to move to another room
        }
    }
    
	// if the arrow hits the wumpus, the player wins the game
    if (finished == win) 
	{
        printf("Wumpus just got whomped. You win!\n");
    }
	
	// if the wumpus gets the player or the arrow hits the player or the player falls in pit, the player loses
    if (finished == lose) 
	{
        printf("HA HA HA - player lose!\n");
    }
    
}//end of void gamePlay()


/*---------------------------------------------------------------------------------------*/

int main() 
{
	displayInfo(); 				// calling function displayInfo()
	
	srand(time(0));				//Seeds a random number using the system time
	
	int gameInProgress = 0;		// The value will stay 0 as far as the game is in progress and has not been won or lost
	int win = 1;				// The values will be 1 once the game is won
	int lose = -1;				// The value will be -1 once the game is lost
	
	do 
	{ 
		gameSetup();
        gamePlay(gameInProgress,win,lose);
	} while (getChar((char *)"New GAME (Y-N)")!= 'N');
    
    return 0;
}
