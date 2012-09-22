#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "getnum.h"

#include "blobsBack.h"

void game(myFile *save,int option);
void graficar( int rowDim , int colDim , char ** table);
int menu(void);
int comando(int move[],char *saveFile);
int saveQuestion(char *saveFile);

int
main(){

    myFile *save;
    char fileName[30],y;
    int option,aux;

	do
	{
		option=menu();
		if(option>4 || option <0)
			printf("ingreso una opcion incorrecta\n");

	}while(option>4 || option <0);
    srand(time(NULL));
    save=malloc(sizeof(myFile));
    if(save==NULL){
        return 0;
    }

	if(option!= END ){
		do
		{	
		if(option==RECOVER_GAME){
		printf("ingrese el nombre del archivo guardado:");
		scanf("%s",fileName);
		BORRAR_BUFFER;
		aux=readBinary(save,fileName);
		if(aux==FILE_ERROR){
			printf("ingreso un nombre de archivo invalido\n");
		}
		}
		}while(aux==FILE_ERROR);
    game(save,option);
	}


    return 0;
}

int
saveQuestion(char *saveFile){

    char ans[3];
    char y;
    char save[]="si";
    char noSave[]="no";
    int aux;
    do
    {
        printf("desea guardar la partida si/no\n");
        scanf("%s",ans);
        BORRAR_BUFFER;
        if(strncmp(ans,save,2)==0){
        printf("ingrese el nombre del archivo a guardar:");
        scanf("%s",saveFile);
        BORRAR_BUFFER;
        aux=COMANDO_SAVE;
        }else if(strncmp(ans,noSave,2)==0){
        aux=COMANDO_QUIT;
        }else
            printf("comando invalido\n");
	if(saveFile==NULL){
		aux!=COMANDO_SAVE;
	}
    }while(aux!=COMANDO_SAVE && aux!=COMANDO_QUIT );

    return aux;
}

void
game(myFile *save,int option){

    int rowDim,colDim,rowO,colO,rowJp,colJp,cmndA;
    int playerTurn=0,aux2,tablero,auxQ,auxS;
    int turn,playerNumber,ans,rad,validPlays,valid;
    char saveFile[30],y;
    int moveSpot[4],cmnd;
    gameStruct *gamePlay;
    computerSpot *chosenSpot;
    player  *aux;

	playerNumber=getPlayerNumber(option);

    if(option!=RECOVER_GAME){
        playerTurn=rand()%PLAYERS;
		do
		{
        printf("ingrese la dimension del tablero Filas x Columnas(5 x 5 minimo):");
        tablero=scanf("%d x %d",&rowDim,&colDim);
        BORRAR_BUFFER;
		if(tablero!=2)
			printf("ingreso una dimension invalida o fuera del formato\n");
		}while((rowDim<5 && colDim<5) || tablero!=2);
        gamePlay=createGameStruct(playerNumber,rowDim,colDim,option);
    }else{
       if(save->gameMode==0){
            playerNumber=2;
        }else{
            playerNumber=1;
        }
        gamePlay=loadPlay(save);
        if(gamePlay==NULL){
         printf("no hay suficiente memoria\n");
        }
    }
        do
        {
            turn=(playerTurn)%PLAYERS;
            gamePlay->playerTurn=turn;
            if(turn==PYER1){
                aux=gamePlay->pyer1;
            }else{
                aux=gamePlay->pyer2;
            }
			graficar(gamePlay->t->rowDim,gamePlay->t->colDim,gamePlay->t->table);
            printf("turno de jugador %d (%c)\n",aux->playerNumber,aux->spotType);
            if(aux->itsHuman==OK){
                do
                {
                    ans=!OK;
                    printf("Accion:");
                    cmnd=comando(moveSpot,saveFile);
                    if(cmnd==COMANDO_MOV){
                        rowO=moveSpot[0];
                        colO=moveSpot[1];
                        rowJp=moveSpot[2];
                        colJp=moveSpot[3];
                        ans=moveValidate(*(gamePlay->t),rowO,colO,rowJp,colJp,*aux);
                        if(ans!=OK)
                            printf("Comando invalido\n");
                    }else if(cmnd==COMANDO_SAVE || cmnd==COMANDO_QUIT){
						cmndA=cmnd;
						do
						{
                        if(cmnd==COMANDO_QUIT){
							cmnd=saveQuestion(saveFile);
							auxQ=COMANDO_QUIT;
                        }
                        if(cmnd==COMANDO_SAVE){
							memoryToSave(save,gamePlay);
							toSave(gamePlay,save,option);
							auxS=writeBinary(save,saveFile);
							ans=!OK;
						
                        }
						if(auxS==FILE_ERROR){
							printf("error en el nombre del archivo\n");
							ans!=OK;
							auxQ!=COMANDO_QUIT;
							if(cmndA!=COMANDO_SAVE){
								cmnd=COMANDO_QUIT;
							}else{
								auxS=!FILE_ERROR;
							}
						}
						}while(auxS==FILE_ERROR);
					}
                }while(ans!=OK && auxQ!=COMANDO_QUIT);
                if(cmnd!=COMANDO_SAVE){
                    if(cmnd==COMANDO_MOV && ans==OK){
                        rad=getRadius(rowO,colO,rowJp,colJp);
                        spotjump(gamePlay->t,rowO,colO,rowJp,colJp,rad);
                        spotInfection(gamePlay->t,rowJp,colJp,*aux);
                    }
                }else
                    aux2=COMANDO_QUIT;
            }else{
                ans=computerInfection(*(gamePlay->t),&chosenSpot);
                if(ans!=EXIT){
                    computerPlay(gamePlay->t,chosenSpot,gamePlay->pyer2);
                }else{
                    valid=adyacentMove(*(gamePlay->t),&chosenSpot,1);
                    if(valid!=EXIT){
                        computerPlay(gamePlay->t,chosenSpot,gamePlay->pyer2);
                    }else{
                        valid=adyacentMove(*(gamePlay->t),&chosenSpot,2);
                        if(valid!=EXIT){
                            computerPlay(gamePlay->t,chosenSpot,gamePlay->pyer2);
                        }
                    }
                }
            }
               validPlays=posiblePlays(*(gamePlay->t),gamePlay->pyer1,gamePlay->pyer2);
               playerTurn++;
        }while(cmnd!=COMANDO_QUIT && validPlays!=0 && aux2!=COMANDO_QUIT);

    if(validPlays==0){
		graficar(gamePlay->t->rowDim,gamePlay->t->colDim,gamePlay->t->table);
        fill(gamePlay->t,*gamePlay);
        graficar(gamePlay->t->rowDim,gamePlay->t->colDim,gamePlay->t->table);
        if(gamePlay->pyer1->spots>gamePlay->pyer2->spots)
            printf("Ha ganado el jugador %d (%c) \n",gamePlay->pyer1->playerNumber,gamePlay->pyer1->spotType);
        else
            printf("Ha ganado el jugador %d (%c) \n",gamePlay->pyer2->playerNumber,gamePlay->pyer2->spotType);
    }
    freeGame(gamePlay);
}

void
graficar( int rowDim , int colDim , char ** table){
   int i,j;

	  putchar('\n');

	  putchar('\t');
	for( j=0; j<colDim ; j++ )
	  {
	     printf(" %d ",j);
	  }

	for( i=0 ; i<rowDim ; i++ )
	   {	putchar('\n');
		printf("%d \t",i);

		for( j=0 ; j<colDim ; j++ )
		{
		   printf("|%c|", table[i][j]);
                }
           }
        putchar('\n');
	putchar('\n');
}

int
menu(void){
     int opcion;
     printf("\n1 - Juego de dos jugadores");
     printf("\n2 - Juego contra la computadora");
     printf("\n3 - Recuperar juego grabado");
     printf("\n4 - Terminar");
     printf("\nElija una opcion:");

	 opcion=getint("");
     return opcion;
}

int
comando(int move[],char *saveFile){

    int i,j;
    int a,b,c,d;
    int valor,aux;
    char guardar[]="save ";
    char salir[]="quit";
    char comandos[MAX_CHAR],y;

    do
    {
    scanf(" %30[^\n]s",comandos);
    BORRAR_BUFFER;
    }while(comandos==NULL);
    valor=sscanf(comandos,"[%d,%d][%d,%d]", &a,&b,&c,&d);

    if( valor == 4 ) {
        move[FILA1]=a;
        move[COLUMNA1]=b;
        move[FILA2]=c;
        move[COLUMNA2]=d;
        aux=COMANDO_MOV;
    }

    if((strncmp(comandos,guardar,5))==0){

        for(i=5,j=0 ; i < MAX_CHAR && comandos[i] ; i++)
        {
            saveFile[j++]=comandos[i];
        }
        saveFile[j]='\0';

        aux=COMANDO_SAVE;
    }

     if( (strncmp(comandos,salir,4)) == 0 ){

	 aux=COMANDO_QUIT;
	}
	return aux;
}
