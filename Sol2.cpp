//Compile with this command:  g++-4.8 -std=c++0x -o Sol2.exe Sol2.cpp

#include <stdio.h>
#include <iostream>     // std::cout
#include <stdlib.h>    
#include <time.h>  
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

using namespace std;

class GameBoard{
  int Deck[52][2]={{0}};//1=spade 2=club 3=heart 4=diamond
  int DeckinPlay[52][2]={{0}};
  int DrawPile[24][2]={{0}};
  int Columns[20][7][2]={{{14}}};
  bool Exposed[20][7]={{0}};
  int Base[13][4]={{0}};
public:
  GameBoard();
  ~GameBoard();
  void PrintCard(int value1,int suit1);
  void PrintDeck();
  void Start();
  void PrintAll();
  void PrintBoard(int showwhich1);
  void askuserwhichmove(int *showwhich1,bool *keepgoing1,bool *end1,int move1[4]);
  void Checkifmoveislegal(int move1[4],int *showwhich1,bool *keepgoing1);
  void MoveCard(int move1[4],int *showwhich1,bool *keepgoing1);    
};
GameBoard::GameBoard(){}
GameBoard::~GameBoard(){
  cout<<"Ended Cleanly"<<endl;
}
void GameBoard::PrintCard(int value1,int suit1){
  if (suit1==0){
    printf(KMAG "%d" RESET ,value1);
  }
  if (suit1==1){
    printf(KBLU "%d" RESET ,value1);
  }
  if (suit1==2){
    printf(KRED "%d" RESET ,value1);
  }
  if (suit1==3){
    printf(KYEL "%d" RESET ,value1);
  }
}
void GameBoard::PrintDeck(){
  for (int i=0;i<52;i++){
    PrintCard(Deck[i][0],Deck[i][1]);
  }
  printf("\n");
}
void GameBoard::Start(){
  //Clear previous Game
  for (int i=0;i<20;i++){
    for (int j=0;j<7;j++){
      Columns[i][j][0]=0;
      Exposed[i][j]=0;
      if (i==j){
	Exposed[i][j]=1;
      }
    }
  }
  //Create Cards
  for (int i=0;i<4;i++){
    for (int j=0;j<13;j++){
      Deck[j+(i*13)][0]=j+1;
      Deck[j+(i*13)][1]=i;
    }
  }
  //PrintDeck();
  //Shuffle
  unsigned seed = clock();
  shuffle (&Deck[0], &Deck[51], default_random_engine(seed));
  cout << "shuffled elements:";
  PrintDeck();
  for (int i=0;i<24;i++){
    DrawPile[i][0] = Deck[i][0];
    DrawPile[i][1] = Deck[i][1];
  }
  bool a=1;
  int card=24;
  int col=0;
  int row=0;
  while(a){
    //printf("Dealing Card: %d",card);
    Columns[row][col][0]=Deck[card][0];
    Columns[row][col][1]=Deck[card][1];
    if (row<col){
      row++;
    }
    else {
      row=0;
      col++;
    }
    card++;
    if (card==52){
      a=0;
    }
  }
}
void GameBoard::PrintAll(){
  printf("DrawPile:\n");
  for (int i=0;i<24;i++){
    PrintCard(DrawPile[i][0],DrawPile[i][1]);  
  }
  printf("\tBase: ");
  /*for (int i=0;i<4;i++){
      PrintCard(Columns[i][j][0],Columns[i][j][1]);
      }*/
  printf("\nColumns:\n");
  for (int i=0;i<20;i++){   
    for (int j=0;j<7;j++){
      if (Columns[i][j][0]==0){
	printf(" ");
      }
      else{
      PrintCard(Columns[i][j][0],Columns[i][j][1]);
      }
      printf("\t");
    }
    printf("\n");
  }
  printf("\n");
}
void GameBoard::PrintBoard(int showwhich1){
  printf("DrawPile:\n");
  for (int i=0;i<24;i++){
    if (showwhich1==i){
      PrintCard(DrawPile[i][0],DrawPile[i][1]);  
    }
    else {
      printf("+");
    }
  }
  printf("\tBase: ");
  for (int i=0;i<4;i++){
    if (Base[i][0]==0){
      printf("x ");
    }
    else{
      PrintCard(Base[i][0],Base[i][1]);
    }
  }
  printf("\nColumns:\n");
  for (int i=0;i<20;i++){   
    for (int j=0;j<7;j++){
      if (Columns[i][j][0]==0){
	printf(" ");
      }
      else if (!Exposed[i][j]){
	printf("+");
      }
      else{
	PrintCard(Columns[i][j][0],Columns[i][j][1]);   
      }
      printf("\t");
    }
    printf("\n");
  }
  printf("\n");
}
void GameBoard::askuserwhichmove(int *showwhich1,bool *keepgoing1,bool *end1,int move1[4]){
  int firstchoice=0;
  printf("Press 1 to look at next card in the deck\nPress 2 to move a card or group of cards\n");
  cin>>firstchoice;
  if (firstchoice==1){
    *showwhich1=*showwhich1+1;
    if (*showwhich1==24){
      *showwhich1=0;
    }
    *keepgoing1=1;
  }
  else if (firstchoice==2){
    //ask which cards to move
    printf("Use 20 for the base rows");
    printf("Which Card to Move?\nWhich Row?\n");
    //*end1=1;
    cin>>move1[0];
    printf("Which Card to Move?\nWhich Col?\n");
    cin>>move1[1];
    printf("To where?\nWhich Row?\n");
    cin>>move1[2];
    printf("To where?\nWhich Col?\n");
    cin>>move1[3];
    *keepgoing1=1;
  }
  else{
    *end1=1;
    *keepgoing1=1;
  }
}
void GameBoard::Checkifmoveislegal(int move1[4],int *showwhich1,bool *keepgoing1){
  *keepgoing1=0; 
  if (move1[2]==20){//its going to a  base
    if (Columns[move1[0]][move1[1]][0]==Columns[move1[2]+1][move1[3]][0]+1){//card is next in ascending order
      if (Columns[move1[0]][move1[1]][1]==Columns[move1[2]+1][move1[3]][1]){//card is same suit
	*keepgoing1=1;
      }}}
  else{ 
    if (Columns[move1[0]][move1[1]][0]==Columns[move1[2]+1][move1[3]][0]-1){//card is next in descending order
      if (Columns[move1[0]][move1[1]][1]==2||Columns[move1[0]][move1[1]][1]==3){    //opposite color
	if (Columns[move1[2]+1][move1[3]][1]==0||Columns[move1[2]+1][move1[3]][1]==1){
	  *keepgoing1=1;	  
	}}
      if (Columns[move1[2]+1][move1[3]][1]==2||Columns[move1[2]+1][move1[3]][1]==2){
	if (Columns[move1[0]][move1[1]][0]==Columns[move1[2]+1][move1[3]][0]-1){//card is next in descending order
	  *keepgoing1=1;	  
	    }}   
    }
  }
}
void GameBoard::MoveCard(int move1[4],int *showwhich1,bool *keepgoing1){    
  Columns[move1[2]+1][move1[3]][0]=Columns[move1[0]][move1[1]][0];//copy card to new location
  Columns[move1[2]+1][move1[3]][1]=Columns[move1[0]][move1[1]][1];
  
  Columns[move1[0]][move1[1]][0]=0;//take away old card
  Columns[move1[0]][move1[1]][1]=0;
}
int main(){
  printf("This is " KMAG "Spades" RESET " This is " KBLU "Clubs" RESET " This is " KRED "Hearts" RESET " This is " KYEL "Diamonds" RESET "\n");
  int showwhich=0;
  bool end=0;
  int moves=1;
  bool keepgoing=0;
  int move[4];
  /*int *sw;
  bool *kg;
  bool *en;
  int *rw,*cl;
  sw =&showwhich;
  kg=&keepgoing;
  en=&end;
  rw=&row;
  cl=&col;*/
  GameBoard PLAY;
  PLAY.Start();
  printf("'+' Are upside down cards\n");
  //PrintAll();
  PLAY.PrintBoard(showwhich);
  //moves=FindMoves();
  //dispmoves();
  while (!end){
    keepgoing=0;
    cout<<"showwhich"<<showwhich<<"keepgoing"<<keepgoing<<"end"<<end<<endl;
    if (moves==0){
      end=1;
    }
    else {
      while (!keepgoing){
	PLAY.askuserwhichmove(&showwhich,&keepgoing,&end,move);
	PLAY.Checkifmoveislegal(move,&showwhich,&keepgoing);
      }
      //moves=FindMoves();
    }
    PLAY.PrintBoard(showwhich);
  }
}

  //Color Stuff
  /*printf(KRED "red\n" RESET);
  printf(KGRN "green\n" RESET);
  printf(KYEL "yellow\n" RESET);
  printf(KBLU "blue\n" RESET);
  printf(KMAG "magenta\n" RESET);
  printf(KCYN "cyan\n" RESET);
  printf(KWHT "white\n" RESET);
  printf(KNRM "normal\n" RESET);*/
  //printf("This is " KRED "red" RESET " and this is " KBLU "blue" RESET "\n");
