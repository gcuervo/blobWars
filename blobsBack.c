#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "blobsBack.h"


void SpotInsert(gameTable *board);

int inThePerimeter(gameTable board,int filO,int colO,int filJp,int colJp,int radius);

int plays(gameTable board,int i,int j);

int arround(gameTable board,int i,int j);

computerSpot computerJump(gameTable board,int i,int j,computerSpot **jSpot,int dim);
computerSpot** computerSpotAnalize(gameTable board,computerSpot **jSpot,int *dim);
int jumpSearch(gameTable board,computerSpot **jSpot);
computerSpot* spotAnalyze(gameTable board,computerSpot **jSpot,int dim);

void fillBoard(gameTable *board,player pyer);

int moveArround(gameTable board,int i,int j,computerSpot *spot,int radius);

void freeInside(computerSpot **cSpot,int dim);

int comando(int move[],char *saveFile);

int readBinary(myFile * f,char * fileName);


gameStruct*
loadPlay(myFile *save){

    int i,j;
    gameStruct *gamePlay;

    gamePlay=malloc(sizeof(gameStruct));
    if(gamePlay==NULL){
        return NULL;
    }
    gamePlay->pyer1=malloc(sizeof(player));
    if(gamePlay->pyer1==NULL){
        return NULL;
    }
    gamePlay->pyer2=malloc(sizeof(player));
    if(gamePlay->pyer2==NULL)
        return NULL;

    gamePlay->pyer1->itsHuman=1;
    gamePlay->pyer1->spotType='A';
    gamePlay->pyer1->playerNumber=1;
    gamePlay->pyer2->spotType='Z';
    gamePlay->pyer2->playerNumber=2;
    if(save->gameMode==1){
    gamePlay->pyer2->itsHuman=0;
    }else{
    gamePlay->pyer2->itsHuman=1;

    }

    gamePlay->t=malloc(sizeof(gameTable));
    if(gamePlay->t==NULL){
        return NULL;
    }
    gamePlay->t->rowDim=save->rowDim;
    gamePlay->t->colDim=save->colDim;
    gamePlay->playerTurn=save->turn;

    gamePlay->t->table=malloc(sizeof(char*)*gamePlay->t->rowDim);
    if(gamePlay->t->table==NULL){
        free(gamePlay->t);
        return NULL;
    }
    for(i=0;i<gamePlay->t->rowDim;i++)
    {
         gamePlay->t->table[i]=malloc(sizeof(char)* gamePlay->t->colDim);
    }

    for(i=0;i<gamePlay->t->rowDim;i++)
        {
            for(j=0;j<gamePlay->t->colDim;j++)
            {
                    gamePlay->t->table[i][j]=save->gameStatus[i][j];

            }
        }

    return gamePlay;
}

int
getPlayerNumber(int option){
	int playerNumber;

	  if(option==1){
        playerNumber=2;
    }else if(option==2){
        playerNumber=1;
    }
	return playerNumber;
}


void
toSave(gameStruct *gamePlay,myFile *save,int option){

	int i,j;

	if(option==2)
		save->gameMode=1;
	else
		save->gameMode=0;
    save->turn=gamePlay->playerTurn;
    save->rowDim=gamePlay->t->rowDim;
    save->colDim=gamePlay->t->colDim;
    for(i=0;i<gamePlay->t->rowDim;i++)
    {
        for(j=0;j<gamePlay->t->colDim;j++)
        {
            save->gameStatus[i][j]=gamePlay->t->table[i][j];
        }
    }
    save->gameStatus=gamePlay->t->table;
    save->player1Blobs=gamePlay->pyer1->spots;
    save->player2Blobs=gamePlay->pyer2->spots;

}

void
memoryToSave(myFile *save,gameStruct *gamePlay){

	int i;

	save->gameStatus=malloc(sizeof(char*)*gamePlay->t->rowDim);

	for(i=0; i<gamePlay->t->rowDim; i++)
	{
		save->gameStatus[i]=malloc(sizeof(char)*gamePlay->t->colDim);
	}
}

int
writeBinary(myFile * f,char * fileName){

    int i,j;
    FILE * miArch;

    if( (miArch=fopen(fileName, "w")) == NULL)
            return FILE_ERROR;

    fwrite(&(f->gameMode),sizeof(int),1,miArch);
    fwrite(&(f->turn),sizeof(int),1,miArch);
    fwrite(&(f->rowDim),sizeof(int),1,miArch);
    fwrite(&(f->colDim),sizeof(int),1,miArch);
    fwrite(&(f->player1Blobs),sizeof(int),1,miArch);
    fwrite(&(f->player2Blobs),sizeof(int),1,miArch);

   for(i=0;i < f->rowDim ; i++)
   {
        for(j=0;j< f->colDim;j++)
        {
            fwrite(&(f->gameStatus[i][j]),sizeof(char),1,miArch);
        }
    }

    fclose(miArch);

 return 0;
 }


int
readBinary(myFile * f,char * fileName){

    int i,j;
    FILE * miArch;

    if( (miArch=fopen(fileName,"r")) == NULL)
        return FILE_ERROR;

    fread(&(f->gameMode),sizeof(int),1,miArch);
    fread(&(f->turn),sizeof(int),1,miArch);
    fread(&(f->rowDim),sizeof(int),1,miArch);
    fread(&(f->colDim),sizeof(int),1,miArch);
    fread(&(f->player1Blobs),sizeof(int),1,miArch);
    fread(&(f->player2Blobs),sizeof(int),1,miArch);

	f->gameStatus=malloc(sizeof(char*)*f->rowDim);
	if(f->gameStatus==NULL)
		return MEMERROR;

    for(i=0; i<f->rowDim; i++)
    {
       f->gameStatus[i]=malloc(sizeof(char)*f->colDim);
    }

    for(i=0;i < f->rowDim;i++)
    {
        for(j=0;j < f->colDim ;j++)
        {
            fread(&(f->gameStatus[i][j]),sizeof(char),1,miArch);
        }
    }

    fclose(miArch);

    return 0;
 }

int
getRadius(int rowO,int colO,int rowJp,int colJp){

    int rad;

    if(abs(rowJp-rowO)>=abs(colJp-colO)){
    rad=abs(rowJp-rowO);
    }else{
    rad=abs(colJp-colO);
    }

    return rad;
}

void
spotjump(gameTable *board,int filO,int colO,int filJp,int colJp,int copy){

    board->table[filJp][colJp]=board->table[filO][colO];

    if(copy==NO){
    board->table[filO][colO]='0';
    }
}

void
SpotInsert(gameTable *board){

    board->table[0][0]='A';
    board->table[board->rowDim-1][0]='A';
    board->table[0][board->colDim-1]='Z';
    board->table[board->rowDim-1][board->colDim-1]='Z';
}

int
moveValidate(gameTable board,int filO,int colO,int filJp,int colJp,player p){

    int flag=OK;


    if((abs(filJp-filO)>2 || abs(colJp-colO)>2) || (abs(filJp-filO)==0 && abs(colJp-colO)==0)){
    flag=INVALID;
    }

    if(filJp<0 && filJp>=board.rowDim && colJp<0 && colJp>=board.colDim){
    flag=INVALID;
    }
    if(board.table[filO][colO]!=p.spotType){

    flag=INVALID;
    }

    if(board.table[filJp][colJp]!='0'){
        flag=INVALID;
    }

    return flag;
}

gameTable*
createTable(int rowDim,int colDim){

    gameTable *board;
    int i,j;

    board=malloc(sizeof(gameTable));
    if(board==NULL){
        return NULL;
    }

    board->rowDim=rowDim;
    board->colDim=colDim;

    board->table=malloc(sizeof(char*)*rowDim);
    if(board->table==NULL){
        free(board);
        return NULL;
    }

    for(i=0;i<rowDim;i++)
    {
        board->table[i]=malloc(sizeof(char)*colDim);
    }

    for(i=0;i<rowDim;i++)
    {
        for(j=0;j<colDim;j++)
        {
            board->table[i][j]='0';
        }
    }

    SpotInsert(board);

    return board;
}

int
inThePerimeter(gameTable board,int filO,int colO,int filJp,int colJp,int radius){

    int aux=OUT;

    if(filJp<board.rowDim && filJp>=0 && colJp<board.colDim && colJp>=0){

        if(abs(filJp-filO)==radius || abs(colJp-colO)==radius){
        aux=IN;
        }
    }

    return aux;
}

/*
infecta las manchas enemigas que estan alrededor del lugar al cual cayo
*/
int
spotInfection(gameTable *board,int rowO,int colO,player p){

    int i,j;
    int ans=NO;

    for(i=rowO-1;i<=rowO+1;i++)
    {
        for(j=colO-1;j<=colO+1;j++)
        {
            if(inThePerimeter(*board,rowO,colO,i,j,1)){
                if(board->table[i][j]!=p.spotType && board->table[i][j]!='0'){
                board->table[i][j]=p.spotType;
                ans=INFECTED;
                }
            }
        }
    }

    return ans;
}

void
printTable(gameTable board){

    int i,j;

    for(i=0;i<board.rowDim;i++)
    {
        for(j=0;j<board.colDim;j++)
        {
            printf("%c\t",board.table[i][j]);
        }
        printf("\n");
    }

}

int
posiblePlays(gameTable board,player *p1,player *p2){

    int i,j;
    int radius=2,ans=0;
    int spots1=0,spots2=0;
    int count1=0,count2=0;

    p1->availablePlays=count1;
    p2->availablePlays=count2;

    for(i=0;i<board.rowDim;i++)
    {
        for(j=0;j<board.colDim;j++)
        {
            radius=2;
            if(board.table[i][j]==p1->spotType){
                spots1++;
                count1=plays(board,i,j);
            }else if(board.table[i][j]==p2->spotType){
                spots2++;
                count2=plays(board,i,j);
            }
            p1->availablePlays+=count1;
            p2->availablePlays+=count2;

            count1=0;
            count2=0;
        }
    }

    p1->spots=spots1;
    p2->spots=spots2;

    if(p1->availablePlays!=0 && p2->availablePlays!=0){
    ans=p1->availablePlays+p2->availablePlays;
    }

    return  ans;
}
/*
devuelve la cantidad de movimientos disponibles sobre 1 mancha
*/
int
plays(gameTable board,int i,int j){

    int count=0;
    int radius=2;
    int k,t;

    while(radius)
    {
        for(k=i-radius;k<=i+radius;k++)
        {
            for(t=j-radius;t<=j+radius;t++)
            {
                if(inThePerimeter(board,i,j,k,t,radius)){
                    if(board.table[k][t]=='0'){

                        count++;
                    }
                }
            }
        }
    radius--;
    }

    return count;
}
/*
devuelve la dimension del vector de saltos que
atrapen enemigos y tambien el vector en el parametro de entrada/salida
*/
int
jumpSearch(gameTable board,computerSpot **jSpot){

    computerSpot **aux;
    int i,j,k=0,l;
    int enemies=0;

    for(i=0;i<board.rowDim;i++)
    {
        for(j=0;j<board.colDim;j++)
        {
            if(board.table[i][j]=='0'){
                enemies=arround(board,i,j);
                if(enemies!=0){
                    jSpot[k]->enemiesAround=enemies;
                    jSpot[k]->rowPosition=i;
                    jSpot[k]->colPosition=j;
                    k++;
                    if(k%BLOCK==0){
                        aux=realloc(jSpot,sizeof(computerSpot*)*(BLOCK+k));
                        if(aux!=NULL){
                            jSpot=aux;
                            for(l=k+1;l<BLOCK+k;l++)
                            {
                                jSpot[l]=malloc(sizeof(computerSpot));
                            }
                        }else{
							freeInside(jSpot,k);
                            return MEMERROR;
						}
                    }
                }

            }
        }
    }


    return k;
}
/*
devuelve la cantidad de manchas posibles a convertir
*/

int
arround(gameTable board,int i,int j){
   int k,t;
   int count=0;
   int radius=1;

    for(k=i-1;k<=i+1;k++)
    {
        for(t=j-1;t<=j+1;t++)
        {
            if(inThePerimeter(board,i,j,k,t,radius)){

                if(board.table[k][t]=='A'){
                    count++;
                }
            }
        }
    }

    return count;
}

/*
devuelve un random de los mejores lugares para
saltar de cada mancha

*/

computerSpot*
spotAnalyze(gameTable board,computerSpot **jSpot,int dim){

    int i,max,k=0,random;
    computerSpot *maximum,*auxSpot;

    maximum=malloc(sizeof(computerSpot)*BLOCK);
    max=jSpot[0]->enemiesAround;

    for(i=1;i<dim;i++)
    {
        if(jSpot[i]->enemiesAround>=max){
        max=jSpot[i]->enemiesAround;
        }
    }

    for(i=0;i<dim;i++)
    {
        if(jSpot[i]->enemiesAround==max){
            maximum[k].rowPosition=jSpot[i]->rowPosition;
            maximum[k].colPosition=jSpot[i]->colPosition;
            maximum[k].spotRowPosition=jSpot[i]->spotRowPosition;
            maximum[k].spotColPosition=jSpot[i]->spotColPosition;
            maximum[k].enemiesAround=jSpot[i]->enemiesAround;
            k++;
            if(k%BLOCK==0){
                auxSpot=realloc(maximum,sizeof(computerSpot)*(BLOCK+k));
                if(auxSpot!=NULL){
                    maximum=auxSpot;
                }else
                    return  NULL;
            }
        }
    }

    if(k!=0)
        random=rand()%k;
    else
        random=0;

    return &maximum[random];
}
/*
recibe el vector de punteros a estructura de lugares a saltar, busca
las manchas de la computadora y ve cuales son las posibles
devuelve un vector de punteros a estructura de los que son posibles

*/
computerSpot**
computerSpotAnalize(gameTable board,computerSpot **jSpot,int *dim){

    int i,j,k=0,t,m;
    computerSpot **cSpot,**aux;

    cSpot=malloc(sizeof(computerSpot*)*BLOCK);
    if(jSpot==NULL){
        return NULL;
    }

    for(t=0;t<BLOCK;t++)
    {
        cSpot[t]=malloc(sizeof(computerSpot));
    }

    for(i=0;i<board.rowDim;i++)
    {
        for(j=0;j<board.colDim;j++)
        {
            if(board.table[i][j]=='Z'){
                *cSpot[k]=computerJump(board,i,j,jSpot,*dim);
                if(cSpot[k]->enemiesAround!=-1){
                    cSpot[k]->spotRowPosition=i;
                    cSpot[k]->spotColPosition=j;
                    k++;
					if(k%BLOCK==0){
						aux=realloc(cSpot,sizeof(computerSpot*)*(k+BLOCK));
						if(aux==NULL){
							return NULL;
						}else{
							cSpot=aux;
							for(m=k+1;m<BLOCK+k;m++)
							{
								cSpot[m]=malloc(sizeof(computerSpot));
							}
						}
					}
                }
            }
        }
    }

    *dim=k;
    if(k==0)
        cSpot=NULL;

    return  cSpot;
}
/*
recibe el vector de punteros a estructura de las manchas de la cpu y
devuelve el mejor lugar a saltar
*/
computerSpot
computerJump(gameTable board,int i,int j,computerSpot **jSpot,int dim){

    int radius=2;
    computerSpot **cSpot,**auxSpot,*ans,error;
    int k,t,l=0,u=0,c,m;

    cSpot=malloc(sizeof(computerSpot*)*BLOCK);
    if(cSpot==NULL){
        return error;
    }

    for(c=0;c<BLOCK;c++)
    {
        cSpot[c]=malloc(sizeof(computerSpot));
    }

    cSpot[0]->enemiesAround=-1;

    while(radius)
    {
        for(k=i-radius;k<=i+radius;k++)
        {
            for(t=j-radius;t<=j+radius;t++)
            {
                if(inThePerimeter(board,i,j,k,t,radius)){
                    while(l<dim)
                    {
                        if(k==jSpot[l]->rowPosition && t==jSpot[l]->colPosition){
                            cSpot[u]->rowPosition=k;
                            cSpot[u]->colPosition=t;
                            cSpot[u]->enemiesAround=jSpot[l]->enemiesAround;
                            u++;
                            if(u%BLOCK==0){
                            auxSpot=realloc(cSpot,sizeof(computerSpot)*(BLOCK+u));
                                if(auxSpot!=NULL){
                                    cSpot=auxSpot;
                                    for(m=u+1;m<BLOCK+u;m++)
                                    {
                                        cSpot[m]=malloc(sizeof(computerSpot));
                                    }

                                }else
                                return  error;
                            }
                        }

                        l++;
                    }
                    l=0;
                }
            }
        }
        radius--;
    }

    if(u!=0){
    dim=u;
        ans=spotAnalyze(board,cSpot,dim);
    }else{
        ans=cSpot[0];
    }

    if(u!=0)
        freeInside(cSpot,u);
    else
        freeInside(cSpot,BLOCK);


    return *ans;
}

void
freeInside(computerSpot **cSpot,int dim){

    int i;

    for(i=0;i<dim;i++)
    {
        free(cSpot[i]);
    }

    free(cSpot);
}

int
computerInfection(gameTable board,computerSpot **chosenSpot){
    computerSpot **jSpots,**maximumSpots;
    int dim,ans,i,dimJ;

    jSpots=malloc(sizeof(computerSpot*)*BLOCK);
    if(jSpots==NULL){
        return MEMERROR;
    }

    for(i=0;i<BLOCK;i++)
    {
        jSpots[i]=malloc(sizeof(computerSpot));
    }
    maximumSpots=malloc(sizeof(computerSpot*)*BLOCK);
    if(maximumSpots==NULL){
        return MEMERROR;
    }

    for(i=0;i<BLOCK;i++)
    {
        maximumSpots[i]=malloc(sizeof(computerSpot));
    }

    dim=jumpSearch(board,jSpots);

    dimJ=dim;
    maximumSpots=computerSpotAnalize(board,jSpots,&dim);
    if(maximumSpots!=NULL){
        *chosenSpot=spotAnalyze(board,maximumSpots,dim);
        if((*chosenSpot)->enemiesAround==-1){
            ans=EXIT;
        }else
            ans=OK;
        if(dim!=0 && dim>BLOCK)
            freeInside(maximumSpots,dim);
        else
            freeInside(maximumSpots,BLOCK);
    }else
        ans=EXIT;

    freeInside(jSpots,BLOCK);
    return ans;
}

void
freeGame(gameStruct *g){

    int i;

    free(g->pyer1);
    free(g->pyer2);
    for(i=0;i<g->t->rowDim;i++)
    {
        free(g->t->table[i]);
    }
    free(g->t->table);
    free(g->t);
    free(g);
}

void
fill(gameTable *board,gameStruct p){

    if(p.pyer1->availablePlays==0){
    fillBoard(board,*(p.pyer2));
    }else
    fillBoard(board,*(p.pyer1));
}


void
computerPlay(gameTable *board,computerSpot *chosenSpot,player *p){

    int rad;

    rad=getRadius(chosenSpot->spotRowPosition,chosenSpot->spotColPosition,chosenSpot->rowPosition,chosenSpot->colPosition);
    spotjump(board,chosenSpot->spotRowPosition,chosenSpot->spotColPosition,chosenSpot->rowPosition,chosenSpot->colPosition,rad);
    spotInfection(board,chosenSpot->rowPosition,chosenSpot->colPosition,*p);
}

gameStruct*
createGameStruct(int playerNumber,int rowDim,int colDim,int option){

    gameStruct *gamePlay;
    int random;

    gamePlay=malloc(sizeof(gameStruct));
    if(gamePlay==NULL){
        return NULL;
    }
    gamePlay->pyer1=malloc(sizeof(player));
    if(gamePlay->pyer1==NULL){
        return NULL;
    }
    gamePlay->pyer2=malloc(sizeof(player));
    if(gamePlay->pyer2==NULL){
        return NULL;
    }
    gamePlay->pyer1->itsHuman=1;
    gamePlay->pyer1->spotType='A';
    gamePlay->pyer1->playerNumber=1;
    gamePlay->pyer2->spotType='Z';
    gamePlay->pyer2->playerNumber=2;
    if(playerNumber==1){
    gamePlay->pyer2->itsHuman=0;
    }else{
    gamePlay->pyer2->itsHuman=1;
    }
    if(option!=RECOVER_GAME){
	gamePlay->t=createTable(rowDim,colDim);
    random=rand()%2;
    gamePlay->playerTurn=random;
    gamePlay->numbersOfPlayers=playerNumber;
    }

    return gamePlay;
}

void
fillBoard(gameTable *board,player pyer){

    int i,j;

    for(i=0;i<board->rowDim;i++)
    {
        for(j=0;j<board->colDim;j++)
        {
            if(board->table[i][j]=='0'){
                board->table[i][j]=pyer.spotType;
            }
        }
    }
}

int
adyacentMove(gameTable board,computerSpot **chosenSpot,int radius){

    int i,j,k=0;
    int enemies=0,random;
    computerSpot *spots,*aux;

    spots=malloc(sizeof(computerSpot)*BLOCK);
    if(spots==NULL)
        return MEMERROR;

    for(i=0;i<board.rowDim;i++)
    {
        for(j=0;j<board.colDim;j++)
        {
            if(board.table[i][j]=='Z'){
            enemies=moveArround(board,i,j,&spots[k],radius);
                if((enemies!=NO_AVAILABLE_PLACES && radius==1) || (enemies!=(NO_AVAILABLE_PLACES*2) && radius==2)){
                    k++;
                    if(k%BLOCK==0){
                    aux=realloc(spots,sizeof(computerSpot)*(BLOCK+k));
                    if(aux==NULL){
                        return MEMERROR;
                    }else{
                        spots=aux;
                        }
                    }
                }
            }

        }
    }

    if(k!=0){
        random=rand()%k;
        *chosenSpot=&spots[random];
    }

    return  k;
}

int
moveArround(gameTable board,int i,int j,computerSpot *spot,int radius){

    int k,t,n=0;
    int random;
    int enemies=0;
    move  places[18];

    spot->spotRowPosition=i;
    spot->spotColPosition=j;

    for(k=i-radius;k<=i+radius;k++)
    {
        for(t=j-radius;t<=j+radius;t++)
        {
            if(inThePerimeter(board,i,j,k,t,radius)){
                if(board.table[k][t]=='0'){
                    places[n].rowMove=k;
                    places[n++].colMove=t;
                }else
                    enemies++;
            }
        }
    }

    if(n!=0){
        random=rand()%n;
        spot->rowPosition=places[random].rowMove;
        spot->colPosition=places[random].colMove;
    }

    return enemies;
}
