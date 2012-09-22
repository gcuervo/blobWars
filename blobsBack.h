/* blobsBack.h
 *                   
 * Trabajo Practico Especial - Programacion Imperativa - 1er cuatrimestre 2011
 *                                                                            
 * Programadores:   
 *
 * Cuervo, Gaston.                                                            
 * Park, Alejandro.                                                 
 *                                                     
 *                                                                            
 */    

/* Definiciones de estructuras. */

typedef struct{             
    int rowDim;
    int colDim;
    char **table;
}gameTable;

typedef struct{             
    int availablePlays;
    int playerNumber;
    int itsHuman;
    int spots;
    char spotType;
}player;

typedef struct{
	
	int	errorMemory;
    int rowPosition;               
    int colPosition;               
    int spotRowPosition;              
    int spotColPosition;
    int enemiesAround;
    char spotType;

}computerSpot;

typedef struct{
 int gameMode;
 int turn;
 int rowDim;
 int colDim;
 int player1Blobs;
 int player2Blobs;
 char ** gameStatus;
}myFile;

typedef struct{

    int rowMove;
    int colMove;
}move;

typedef struct{

    player *pyer1;
    player *pyer2;
    gameTable *t;
    int playerTurn;
    int numbersOfPlayers;

}gameStruct;


#define NO                      2
#define OK                      1
#define IN                      1
#define OUT                     0
#define INVALID                 0
#define EXIT                    0
#define END						4
#define INFECTED                -1
#define BLOCK                   30
#define MEMERROR                -12
#define PLAYERS                 2
#define PYER1                   0
#define PYER2                   1
#define NO_AVAILABLE_PLACES     8
#define UN_JUGADOR              1
#define MAX_CHAR                30
#define COMANDO_MOV             20
#define COMANDO_SAVE            21
#define COMANDO_QUIT            22
#define FILA1                   0
#define COLUMNA1                1
#define FILA2                   2
#define COLUMNA2                3
#define RECOVER_GAME            3
#define FILE_ERROR              -10
#define BORRAR_BUFFER      		 while((y=getchar())!='\n')      



int getPlayerNumber(int option);
gameStruct* createGameStruct(int playerNumber,int rowDim,int colDim,int option);
void graficar( int rowDim , int colDim , char ** table);
void memoryToSave(myFile *save,gameStruct *gamePlay);
void toSave(gameStruct *gamePlay,myFile *save,int option);
gameStruct* loadPlay(myFile *save);
int writeBinary(myFile * f,char * fileName);
int readBinary(myFile * f,char * fileName);
int comando(int move[],char *saveFile);
void fill(gameTable *board,gameStruct p);
void computerPlay(gameTable *board,computerSpot *chosenSpot,player *p);
gameStruct* createGameStruct(int playerNumber,int rowDim,int colDim,int option);
int computerInfection(gameTable board,computerSpot **chosenSpot);
int getPlayerNumber(int option);
void freeGame(gameStruct *g);
int posiblePlays(gameTable Board,player *p1,player *p2);
gameTable* createTable(int rowDim,int colDim);
int moveValidate(gameTable board,int filO,int colO,int filJp,int colJp,player p);
int spotInfection(gameTable *board,int rowO,int colO,player p);
void spotjump(gameTable *board,int filO,int colO,int filJp,int colJp,int copy);
int adyacentMove(gameTable board,computerSpot **chosenSpot,int radius);
int getRadius(int rowO,int colO,int rowJp,int colJp);