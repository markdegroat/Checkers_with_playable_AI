//
//  main.cpp
//  Checkers
//
//  Created by 00 on 3/8/15.
//  Copyright (c) 2015 00. All rights reserved.
//
#include <unistd.h>
#include <iostream>
#include <vector>

int rowA = 0;
int rowB = 0;
int colA = 0;
int colB = 0;

struct move{
    public:
        int prev_x;
        int prev_y;
        int next_x;
        int next_y;
  
    
    
    
};



//struct movelist{
//    std::vector<move> list;
//};

int scoreMask[8][8] =//trying to make the edges of the board more desirable so the agent puts pieces in good positions, and wants to reach the opposite side of the board
//took this out of the object because it doesnt change when the board changes
{
    {4,0,4,0,4,0,4,0},
    {0,3,0,3,0,3,0,4},
    {3,0,2,0,2,0,2,0},
    {0,1,0,1,0,1,0,2},
    {2,0,1,0,1,0,1,0},
    {0,2,0,2,0,2,0,3},
    {4,0,3,0,3,0,3,0},
    {0,4,0,4,0,4,0,4},
};




class Board {
    public:

//    char pieces[8][8] =
//    {
//        {' ',' ',' ',' ',' ',' ',' ',' '},
//        {' ','o',' ',' ',' ',' ',' ',' '},
//        {' ',' ',' ',' ',' ',' ',' ',' '},
//        {' ',' ',' ','o',' ',' ',' ',' '},
//        {' ',' ',' ',' ',' ',' ',' ',' '},
//        {' ',' ',' ',' ',' ','o',' ',' '},
//        {' ',' ',' ',' ',' ',' ',' ',' '},
//        {' ',' ',' ',' ',' ',' ',' ','K'},
//    };
    
//    char pieces[8][8] =
//    {
//        {'x',' ','x',' ','x',' ','x',' '},
//        {' ','x',' ','x',' ','x',' ','x'},
//        {' ',' ','x',' ','x',' ','x',' '},
//        {' ',' ',' ',' ',' ',' ',' ',' '},
//        {' ',' ','o',' ',' ',' ',' ',' '},
//        {' ','x',' ',' ',' ','o',' ','o'},
//        {'o',' ','o',' ','o',' ','o',' '},
//        {' ','o',' ',' ',' ','o',' ','o'},
//    };
        char pieces[8][8] =
        {
            {'x',' ','x',' ','x',' ','x',' '},
            {' ','x',' ','x',' ','x',' ','x'},
            {'x',' ','x',' ','x',' ','x',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ',' ',' ',' ',' ',' ',' ',' '},
            {' ','o',' ','o',' ','o',' ','o'},
            {'o',' ','o',' ','o',' ','o',' '},
            {' ','o',' ','o',' ','o',' ','o'},
        };
    
    

    int board_score = 0;
    char whosTurn = 'x';
    bool gameOn = true;
    bool justJumped = false;
    bool wasJustKinged = false;
    int piece_jumped_x = 0;
    int piece_jumped_y = 0;
    char piece_jumped = ' ';
    int number_of_steps = 0;
    

  
    void makeMove (int, int, int, int);
    void undoMove (int, int ,int, int);
    void undoMove (move);
    bool canMove ( int, int, int ,int);
    void printBoard(Board);
    char getPiece(int ,int);
    void setPiece(int, int, char);
    void changeTurn();
    void setTurn(char);
    bool checkGameOver();
    bool checkIfWon();
    void askMove();
    bool canJump2(int, int);
    bool canJump(int, int, int, int);
    int xScore();
    int oScore();
    int xScor();
    int oScor();
    int kScor();
    int qScor();
    int evaluateBoard();
    std::vector<move> findAllMoves();

};

void Board::changeTurn(){
    if (this->whosTurn=='x') {
        this->whosTurn='o';
        number_of_steps++;
    }
    else{
        this->whosTurn='x';
        number_of_steps++;
    }
}

int Board::evaluateBoard(){
    int sum = 0;
    int royal_weight = 2; //change this to make getting kinged more desirable
    for(int i=0;i < 8; i++){
        for(int j=0;j < 8; j++){
            if(this->pieces[i][j]== 'x'){//x's add w/e the weight is at the mask
                sum = sum + (scoreMask[i][j]);
            }
            if(this->pieces[i][j]== 'o'){//o's sub w/e the weight is at the mask
                sum = sum - (scoreMask[i][j]);
            }
            if(this->pieces[i][j]== 'K'){//K's add w/e the weight is at the mask time there royal weight
                sum = sum + (royal_weight * scoreMask[i][j]);
            }
            if(this->pieces[i][j]== 'Q'){//Q's  sub w/e the weight is at the mask time there royal weight
                sum = sum - (royal_weight * scoreMask[i][j]);
            }
            
        }
    }
    return sum;
}
void Board::setTurn(char piece){
    this->whosTurn= piece;
}


void Board::makeMove(int selRow, int selCol, int tarRow, int tarCol) {
    
    if( (selRow>=0 && selRow<=7) && (selCol>=0 && selCol<=7) && (tarRow>=0 && tarRow<=7) && (tarCol>=0 && tarCol<=7)){
        
    
   
    char pieceToMove = this->getPiece(selRow, selCol);
    //std::cout<< "This is the selected piece:" << getPiece(selRow,selCol)<<"\n";
   //std::cout<< "This is the target piece:" << getPiece(tarRow,tarCol)<<"\n";
    
    //limiting the row difference to only be negative means x can only move down the board (until kinged)
    
    if(pieceToMove==' '){
        std::cout << "There is not currently a piece in that place of the board." << std::endl;
        //throw error to be handled
    
    }
    
    else if(pieceToMove=='x'){
        if(this->whosTurn=='x'){
            if(this->getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 1)||(tarCol-selCol == -1))&&((tarRow-selRow == 1))){
                if(tarRow == 7){
                    this->setPiece(tarRow, tarCol, 'K');
                    this->setPiece(selRow, selCol, ' ');
                    this->wasJustKinged = true;
                    this->changeTurn();
                }
                else{
                    this->setPiece(tarRow, tarCol, pieceToMove);
                    this->setPiece(selRow, selCol, ' ');
                    this->wasJustKinged = false;
                    this->changeTurn();
                }
            
            }
            else if (this->getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 2)||(tarCol-selCol == -2)) && (tarRow-selRow == 2) && (this->canJump(selRow,selCol,tarRow,tarCol))){
                if(tarRow == 7){
                    this->setPiece(tarRow, tarCol, 'K');
                    this->setPiece(selRow, selCol, ' ');
                    this->wasJustKinged = true;
                    this->changeTurn();
                    
                }
                else{
                    this->setPiece(tarRow, tarCol, pieceToMove);
                    this->setPiece(selRow, selCol, ' ');
                    this->wasJustKinged = false;
                    this->changeTurn();
                    
                }
                 this->piece_jumped = getPiece((selRow+tarRow)/2,(selCol+tarCol)/2);
                this->setPiece(((tarRow+selRow)/2),((tarCol+selCol)/2), ' ');
                this->justJumped = true;
               
                this->piece_jumped_x = (selRow+tarRow)/2;
                this->piece_jumped_y =(selCol+tarCol)/2;
                //std::cout<< "OMG you got jumped!!!\n";
            }
            else{
                std::cout << "x pieces can't make that move... you jerk...\n";
            }
        }
        else{
            std::cout<<"yo cmon its not x's turn\n";
        }
    }
        
    
    //this is for x's that have been kinged so they can move any direction
    else if(pieceToMove=='K'){
        if(this->whosTurn=='x'){
            if(this->getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 1)||(tarCol-selCol == -1))&&((tarRow-selRow == 1) || (tarRow-selRow == -1) )){
                this->setPiece(tarRow, tarCol, pieceToMove);
                this->setPiece(selRow, selCol, ' ');
                this->wasJustKinged = false;
                this->changeTurn();
            }
            else if (this->getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 2)||(tarCol-selCol == -2)) && ((tarRow-selRow == -2)|| (tarRow-selRow == 2) ) && (this->canJump(selRow,selCol,tarRow,tarCol))){
                this->setPiece(tarRow, tarCol, pieceToMove);
                this->setPiece(selRow, selCol, ' ');
                this->wasJustKinged = false;
                
                this->changeTurn();
                
                this->piece_jumped = getPiece((selRow+tarRow)/2,(selCol+tarCol)/2);
                this->setPiece(((tarRow+selRow)/2),((tarCol+selCol)/2), ' ');
                this->justJumped = true;
                
                this->piece_jumped_x = (selRow+tarRow)/2;
                this->piece_jumped_y =(selCol+tarCol)/2;
                
                //std::cout<< "OMG you got jumped!!!\n";
            }
            else{
                std::cout << "K pieces can't make that move... you jerk...\n";
            }
        }
        else{
            std::cout << "yo cmon it's not x's turn..\n";
        }
    }
    else if(pieceToMove=='o'){
        if(this->whosTurn=='o'){
            if(this->getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 1)||(tarCol-selCol == -1))&&((tarRow-selRow == -1))){
                if(tarRow == 0){
                    this->setPiece(tarRow, tarCol, 'Q');
                    this->setPiece(selRow, selCol, ' ');
                    this->wasJustKinged = true;
                    this->changeTurn();
                }
                else{
                this->setPiece(tarRow, tarCol, pieceToMove);
                this->setPiece(selRow, selCol, ' ');
                    this->wasJustKinged = false;
                this->changeTurn();
                }
            }
            else if (this->getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 2)||(tarCol-selCol == -2)) && (tarRow-selRow == -2) && (this->canJump(selRow,selCol,tarRow,tarCol))){
                if(tarRow == 0){
                    this->setPiece(tarRow, tarCol, 'Q');
                    this->setPiece(selRow, selCol, ' ');
                    this->wasJustKinged = true;
                    
                    this->changeTurn();
                    
                }
                else{
                    this->setPiece(tarRow, tarCol, pieceToMove);
                    this->setPiece(selRow, selCol, ' ');
                    this->wasJustKinged = false;
                    
                    this->changeTurn();
                    
                    }
                this->piece_jumped = getPiece((selRow+tarRow)/2,(selCol+tarCol)/2);
                this->setPiece(((tarRow+selRow)/2),((tarCol+selCol)/2), ' ');
                this->justJumped = true;
                
                this->piece_jumped_x = (selRow+tarRow)/2;
                this->piece_jumped_y =(selCol+tarCol)/2;
                //std::cout<< "OMG you got jumped!!!\n";
            }

            else{
                std::cout << "0 pieces can't make that move... you jerk...\n";
            }
        }
        else{
            std::cout<<"yo cmon it's not o's turn\n";
        }
    }
    
    else if(pieceToMove=='Q'){
        if(this->whosTurn=='o'){
        if(this->getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 1)||(tarCol-selCol == -1))&&((tarRow-selRow == 1) || (tarRow-selRow == -1) )){
            this->setPiece(tarRow, tarCol, pieceToMove);
            this->setPiece(selRow, selCol, ' ');
            this->wasJustKinged = false;
            this->changeTurn();
        }
        else if (this->getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 2)||(tarCol-selCol == -2)) && ((tarRow-selRow == -2)|| (tarRow-selRow == 2) ) && (this->canJump(selRow,selCol,tarRow,tarCol))){
            this->setPiece(tarRow, tarCol, pieceToMove);
            this->setPiece(selRow, selCol, ' ');
            this->piece_jumped = getPiece((selRow+tarRow)/2,(selCol+tarCol)/2);
            this->setPiece(((tarRow+selRow)/2),((tarCol+selCol)/2), ' ');
            this->justJumped = true;
            
            this->piece_jumped_x = (selRow+tarRow)/2;
            this->piece_jumped_y =(selCol+tarCol)/2;
            this->wasJustKinged = false;
           
            this->changeTurn();
            
            //std::cout<< "OMG you got jumped!!!\n";
        }
        else{
            std::cout << "Q pieces can't make that move... you jerk...\n";
        }
        }
        else{
            std::cout<<"yo cmon it's not o's turn\n";
        }
    }
        
    }//end of initial if
    else{
        std::cout << "Please enter a number that is between 0 and 7 for the values.." << std::endl;
    }
    

}
void Board::undoMove(int selRow, int selCol, int tarRow, int tarCol){
    char pieceToMove = this->getPiece(tarRow, tarCol);
    
    if(pieceToMove=='x'){

        if(!this->justJumped){
                    this->setPiece(tarRow, tarCol, ' ');
                    this->setPiece(selRow, selCol, pieceToMove);
                    this->changeTurn();
        }
        else{//just Jumped
            this->setPiece(tarRow, tarCol, ' ');
            this->setPiece(selRow, selCol, pieceToMove);
            this->setPiece((tarRow+selRow)/2,(tarCol+selCol)/2, this->piece_jumped);//sets back the piece that was just jumped
            this->changeTurn();
        }
        
        
    }
    
    else if(pieceToMove=='o'){
        
        if(!this->justJumped){
            this->setPiece(tarRow, tarCol, ' ');
            this->setPiece(selRow, selCol, pieceToMove);
            this->changeTurn();
        }
        else{//just Jumped
            this->setPiece(tarRow, tarCol, ' ');
            this->setPiece(selRow, selCol, pieceToMove);
            this->setPiece((tarRow+selRow)/2,(tarCol+selCol)/2, this->piece_jumped);//sets back the piece that was just jumped
            this->changeTurn();
        }
        
        
    }
    
    else if(pieceToMove=='K'){
        
    if(this->wasJustKinged){
        
        if(!this->justJumped){
            this->setPiece(tarRow, tarCol, ' ');
            this->setPiece(selRow, selCol, 'x');
            this->changeTurn();
        }
        else{//just Jumped
            this->setPiece(tarRow, tarCol, ' ');
            this->setPiece(selRow, selCol, 'x');
            this->setPiece((tarRow+selRow)/2,(tarCol+selCol)/2, this->piece_jumped);//sets back the piece that was just jumped
            this->changeTurn();
        }
    }
    else{
        if(!this->justJumped){
            this->setPiece(tarRow, tarCol, ' ');
            this->setPiece(selRow, selCol, pieceToMove);
            this->changeTurn();
        }
        else{//just Jumped
            this->setPiece(tarRow, tarCol, ' ');
            this->setPiece(selRow, selCol, pieceToMove);
            this->setPiece((tarRow+selRow)/2,(tarCol+selCol)/2, this->piece_jumped);//sets back the piece that was just jumped
            this->changeTurn();
        }
    }
        
        
    }
    
    else if(pieceToMove=='Q'){
        
        if(this->wasJustKinged){
            
            if(!this->justJumped){
                this->setPiece(tarRow, tarCol, ' ');
                this->setPiece(selRow, selCol, 'o');
                this->changeTurn();
            }
            else{//just Jumped
                this->setPiece(tarRow, tarCol, ' ');
                this->setPiece(selRow, selCol, 'o');
                this->setPiece((tarRow+selRow)/2,(tarCol+selCol)/2, this->piece_jumped);//sets back the piece that was just jumped
                this->changeTurn();
            }
        }
        else{
            if(!this->justJumped){
                this->setPiece(tarRow, tarCol, ' ');
                this->setPiece(selRow, selCol, pieceToMove);
                this->changeTurn();
            }
            else{//just Jumped
                this->setPiece(tarRow, tarCol, ' ');
                this->setPiece(selRow, selCol, pieceToMove);
                this->setPiece((tarRow+selRow)/2,(tarCol+selCol)/2, this->piece_jumped);//sets back the piece that was just jumped
                this->changeTurn();
            }
        }
        
        
    }
    else{
        std::cout << "error during undo" << std::endl;
    }

    
    


    
    
}


std::vector<move> Board::findAllMoves(){//returns a vector with all the possible moves for the current board
    std::vector<move> moveList;
    for(int i = 0;i < 8; i++){
        for(int j = 0;j < 8; j++){
            if(this->pieces[i][j]=='x' && this->whosTurn == 'x'){
                if(canMove(i,j,i+1,j-1)){
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+1;
                    next.next_y = j-1;
                    moveList.push_back(next);
                    
                    //add to list
                }
                if(canMove(i,j,i+1,j+1)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+1;
                    next.next_y = j+1;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i+2,j-2) && canJump(i, j, i+2, j-2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+2;
                    next.next_y = j-2;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i+2,j+2) && canJump(i,j,i+2,j+2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+2;
                    next.next_y = j+2;
                    moveList.push_back(next);
                }

            }
            
            else if(this->pieces[i][j]=='o' && this->whosTurn == 'o'){
                if(canMove(i,j,i-1,j-1)){
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-1;
                    next.next_y = j-1;
                    moveList.push_back(next);
                    
                    //add to list
                }
                if(canMove(i,j,i-1,j+1)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-1;
                    next.next_y = j+1;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i-2,j-2) && canJump(i, j, i-2, j-2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-2;
                    next.next_y = j-2;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i-2,j+2) && canJump(i,j,i-2,j+2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-2;
                    next.next_y = j+2;
                    moveList.push_back(next);
                }
                
            }
           else if(this->pieces[i][j]=='K' && this->whosTurn == 'x'){
                if(canMove(i,j,i+1,j-1)){
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+1;
                    next.next_y = j-1;
                    moveList.push_back(next);
                    
                    //add to list
                }
                if(canMove(i,j,i+1,j+1)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+1;
                    next.next_y = j+1;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i+2,j-2) && canJump(i, j, i+2, j-2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+2;
                    next.next_y = j-2;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i+2,j+2) && canJump(i,j,i+2,j+2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+2;
                    next.next_y = j+2;
                    moveList.push_back(next);
                }
                
                if(canMove(i,j,i-1,j-1)){
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-1;
                    next.next_y = j-1;
                    moveList.push_back(next);
                    
                    //add to list
                }
                if(canMove(i,j,i-1,j+1)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-1;
                    next.next_y = j+1;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i-2,j-2) && canJump(i, j, i-2, j-2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-2;
                    next.next_y = j-2;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i-2,j+2) && canJump(i,j,i-2,j+2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-2;
                    next.next_y = j+2;
                    moveList.push_back(next);
                }
                
            }
            else if(this->pieces[i][j]=='Q' && this->whosTurn == 'o'){
                if(canMove(i,j,i+1,j-1)){
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+1;
                    next.next_y = j-1;
                    moveList.push_back(next);
                    
                    //add to list
                }
                if(canMove(i,j,i+1,j+1)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+1;
                    next.next_y = j+1;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i+2,j-2) && canJump(i, j, i+2, j-2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+2;
                    next.next_y = j-2;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i+2,j+2) && canJump(i,j,i+2,j+2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i+2;
                    next.next_y = j+2;
                    moveList.push_back(next);
                }
                
                if(canMove(i,j,i-1,j-1)){
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-1;
                    next.next_y = j-1;
                    moveList.push_back(next);
                    
                    //add to list
                }
                if(canMove(i,j,i-1,j+1)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-1;
                    next.next_y = j+1;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i-2,j-2) && canJump(i, j, i-2, j-2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-2;
                    next.next_y = j-2;
                    moveList.push_back(next);
                }
                if(canMove(i,j,i-2,j+2) && canJump(i,j,i-2,j+2)){
                    //add to list
                    move next;
                    next.prev_x = i;
                    next.prev_y = j;
                    next.next_x = i-2;
                    next.next_y = j+2;
                    moveList.push_back(next);
                }
                
            }
        }
    }
    
    return moveList;
    
}

bool Board::canMove(int selRow, int selCol, int tarRow, int tarCol) {
    
    if( (selRow>=0 && selRow<=7) && (selCol>=0 && selCol<=7) && (tarRow>=0 && tarRow<=7) && (tarCol>=0 && tarCol<=7)){
        
        
        
        char pieceToMove = getPiece(selRow, selCol);

        
        //limiting the row difference to only be negative means x can only move down the board (until kinged)
        
        if(pieceToMove==' '){
            return false;
            
        }
        
        else if(pieceToMove=='x'){
            if(this->whosTurn=='x'){
                if(getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 1)||(tarCol-selCol == -1))&&((tarRow-selRow == 1))){
                    return true;
                    
                }
                else if (getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 2)||(tarCol-selCol == -2)) && (tarRow-selRow == 2) && (canJump(selRow,selCol,tarRow,tarCol))){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        
        
        //this is for x's that have been kinged so they can move any direction
        else if(pieceToMove=='K'){
            if(this->whosTurn=='x'){
                if(getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 1)||(tarCol-selCol == -1))&&((tarRow-selRow == 1) || (tarRow-selRow == -1) )){
                    return true;
                }
                else if (getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 2)||(tarCol-selCol == -2)) && ((tarRow-selRow == -2)|| (tarRow-selRow == 2) ) && (canJump(selRow,selCol,tarRow,tarCol))){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        else if(pieceToMove=='o'){
            if(this->whosTurn=='o'){
                if(getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 1)||(tarCol-selCol == -1))&&((tarRow-selRow == -1))){
                    return true;
                }
                else if (getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 2)||(tarCol-selCol == -2)) && (tarRow-selRow == -2) && (canJump(selRow,selCol,tarRow,tarCol))){
                    return true;
                }
                
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        
        else if(pieceToMove=='Q'){
            if(this->whosTurn=='o'){
                if(getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 1)||(tarCol-selCol == -1))&&((tarRow-selRow == 1) || (tarRow-selRow == -1) )){
                    return true;
                }
                else if (getPiece(tarRow, tarCol) == ' ' && ((tarCol-selCol == 2)||(tarCol-selCol == -2)) && ((tarRow-selRow == -2)|| (tarRow-selRow == 2) ) && (canJump(selRow,selCol,tarRow,tarCol))){
                    return true;
                }
                else{
                    return false;
                }
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
        
    }//end of initial if
    else{
        return false;
    }
    
    
}
bool Board::canJump2(int x, int y){
    if( (getPiece(x,y) == 'x') && (((getPiece(x+1,y-1)==('o') || getPiece(x+1,y-1)==('Q')) && getPiece(x+2,y-2)==' ') || ((getPiece(x+1,y+1)=='o'||getPiece(x+1,y+1)=='Q' ) && getPiece(x+2,y+2)==' ') ) ){
        return true;
    }
    else if( (getPiece(x,y) == 'o') && (((getPiece(x-1,y-1)=='x'||getPiece(x-1,y-1)=='K') && getPiece(x-2,y-2)==' ') || ((getPiece(x-1,y+1)=='x'||getPiece(x-1,y+1)=='K') && getPiece(x-2,y+2)==' ') ) ){
        return true;
    }
    else if( (getPiece(x,y) == 'K') && (((getPiece(x+1,y-1)==('o') || getPiece(x+1,y-1)==('Q')) && getPiece(x+2,y-2)==' ') || ((getPiece(x+1,y+1)=='o'||getPiece(x+1,y+1)=='Q' ) && getPiece(x+2,y+2)==' ') || (((getPiece(x-1,y-1)=='o'||getPiece(x-1,y-1)=='Q') && getPiece(x-2,y-2)==' ') || ((getPiece(x-1,y+1)=='o'||getPiece(x-1,y+1)=='Q') && getPiece(x-2,y+2)==' ') )) ){
        return true;
    }
    else if( (getPiece(x,y) == 'Q') && (((getPiece(x-1,y-1)=='x'||getPiece(x-1,y-1)=='K') && getPiece(x-2,y-2)==' ') || ((getPiece(x-1,y+1)=='x'||getPiece(x-1,y+1)=='K') && getPiece(x-2,y+2)==' ')|| (((getPiece(x+1,y-1)==('x') || getPiece(x+1,y-1)==('K')) && getPiece(x+2,y-2)==' ') || ((getPiece(x+1,y+1)=='x'||getPiece(x+1,y+1)=='K' ) && getPiece(x+2,y+2)==' ') )  )  ){
        return true;
    }
    else{
        return false;
    }


}

bool Board::canJump(int x, int y, int tarRow, int tarCol){
    
   
    
    if(((getPiece(x,y) == 'x') || (getPiece(x,y) == 'K')) && ((getPiece((x+tarRow)/2,(y+tarCol)/2) == ('o')) ||  (getPiece((x+tarRow)/2,(y+tarCol)/2) == ('Q'))))
       {
        return true;
    }
    else if(((getPiece(x,y) == 'o') || (getPiece(x,y) == 'Q')) && ((getPiece((x+tarRow)/2,(y+tarCol)/2) == ('x')) ||  (getPiece((x+tarRow)/2,(y+tarCol)/2) == ('K'))))
    {
        return true;
    }
  

    else{
        return false;
    }
    
}
void Board::printBoard(Board game){
    std::cout << "  |0|1|2|3|4|5|6|7|"<<std::endl;
    std::cout << "|0|"<<game.pieces[0][0]<<"|"<<game.pieces[0][1]<<"|"<<game.pieces[0][2]<<"|"<<game.pieces[0][3]<<"|"<<game.pieces[0][4]<<"|"<<game.pieces[0][5]<<"|"<<game.pieces[0][6]<<"|"<<game.pieces[0][7]<<"|\n";
    std::cout << "|1|"<<game.pieces[1][0]<<"|"<<game.pieces[1][1]<<"|"<<game.pieces[1][2]<<"|"<<game.pieces[1][3]<<"|"<<game.pieces[1][4]<<"|"<<game.pieces[1][5]<<"|"<<game.pieces[1][6]<<"|"<<game.pieces[1][7]<<"|\n";
    std::cout << "|2|"<<game.pieces[2][0]<<"|"<<game.pieces[2][1]<<"|"<<game.pieces[2][2]<<"|"<<game.pieces[2][3]<<"|"<<game.pieces[2][4]<<"|"<<game.pieces[2][5]<<"|"<<game.pieces[2][6]<<"|"<<game.pieces[2][7]<<"|\n";
    std::cout << "|3|"<<game.pieces[3][0]<<"|"<<game.pieces[3][1]<<"|"<<game.pieces[3][2]<<"|"<<game.pieces[3][3]<<"|"<<game.pieces[3][4]<<"|"<<game.pieces[3][5]<<"|"<<game.pieces[3][6]<<"|"<<game.pieces[3][7]<<"|\n";
    std::cout << "|4|"<<game.pieces[4][0]<<"|"<<game.pieces[4][1]<<"|"<<game.pieces[4][2]<<"|"<<game.pieces[4][3]<<"|"<<game.pieces[4][4]<<"|"<<game.pieces[4][5]<<"|"<<game.pieces[4][6]<<"|"<<game.pieces[4][7]<<"|\n";
    std::cout << "|5|"<<game.pieces[5][0]<<"|"<<game.pieces[5][1]<<"|"<<game.pieces[5][2]<<"|"<<game.pieces[5][3]<<"|"<<game.pieces[5][4]<<"|"<<game.pieces[5][5]<<"|"<<game.pieces[5][6]<<"|"<<game.pieces[5][7]<<"|\n";
    std::cout << "|6|"<<game.pieces[6][0]<<"|"<<game.pieces[6][1]<<"|"<<game.pieces[6][2]<<"|"<<game.pieces[6][3]<<"|"<<game.pieces[6][4]<<"|"<<game.pieces[6][5]<<"|"<<game.pieces[6][6]<<"|"<<game.pieces[6][7]<<"|\n";
    std::cout << "|7|"<<game.pieces[7][0]<<"|"<<game.pieces[7][1]<<"|"<<game.pieces[7][2]<<"|"<<game.pieces[7][3]<<"|"<<game.pieces[7][4]<<"|"<<game.pieces[7][5]<<"|"<<game.pieces[7][6]<<"|"<<game.pieces[7][7]<<"|\n";
    std::cout << "\n";
}
char Board::getPiece(int x, int y){
    char piece = this->pieces[x][y];
    return piece;
}
void Board::setPiece(int x, int y, char piece){
    this->pieces[x][y] = piece;
}
int Board::xScore(){
    int scoreToReturn = 0;
    for(int x =0; x < 8;x++){
        for(int y = 0; y<8; y++){
            if(this->pieces[x][y] == 'x' || this->pieces[x][y] == 'K'){
                scoreToReturn++;
            }
        }
    }
    return scoreToReturn;
}
int Board::oScore(){
    int scoreToReturn = 0;
    for(int x =0; x < 8;x++){
        for(int y = 0; y<8; y++){
            if(this->pieces[x][y] == 'o' || this->pieces[x][y] == 'Q'){
                scoreToReturn++;
            }
        }
    }
    return scoreToReturn;
}
int Board::xScor(){
    int scoreToReturn = 0;
    for(int x =0; x < 8;x++){
        for(int y = 0; y<8; y++){
            if(this->pieces[x][y] == 'x'){
                scoreToReturn++;
            }
        }
    }
    return scoreToReturn;
} // these only count o's and x's, not total score for side "x" or side "o"
int Board::oScor(){
    int scoreToReturn = 0;
    for(int x =0; x < 8;x++){
        for(int y = 0; y<8; y++){
            if(this->pieces[x][y] == 'o'){
                scoreToReturn++;
            }
        }
    }
    return scoreToReturn;
} // these only count o's and x's, not total score for side "x" or side "o"
int Board::kScor(){
    int scoreToReturn = 0;
    for(int x =0; x < 8;x++){
        for(int y = 0; y<8; y++){
            if(this->pieces[x][y] == 'K'){
                scoreToReturn++;
            }
        }
    }
    return scoreToReturn;
}
int Board::qScor(){
    int scoreToReturn = 0;
    for(int x =0; x < 8;x++){
        for(int y = 0; y<8; y++){
            if(this->pieces[x][y] == 'Q'){
                scoreToReturn++;
            }
        }
    }
    return scoreToReturn;
}
bool Board::checkGameOver(){
    if(xScore()>=1 && oScore()<1)
    {
        return true;
    }
    else if(xScore()<1 && oScore()>=1)
    {
        return true;
    }
    else{
        return false;
    }
    
}
bool Board::checkIfWon(){
    if(xScore()>=1 && oScore()<1)
    {
        return true;
    }
  
    else{
        return false;
    }
    
}
void Board::askMove(){
    int checkRowA = 0;
    int checkRowB = 0;
    int checkColA = 0;
    int checkColB = 0;
    std::cout << "please type in the row of your piece, then the col of your piece" << std::endl;
    
    std::cin >> checkRowA;
    
    while(checkRowA<0 || checkRowA>7){
        std::cout << "you idiot... try again with a number thats in the range of 0 to 7." << std::endl;
        std::cin >> checkRowA;
    }
    rowA = checkRowA;
    
    std::cin >> checkColA;
    
    while(checkColA<0 || checkColA>7){
        std::cout << "you idiot... try again with a number thats in the range of 0 to 7." << std::endl;
        std::cin >> checkColA;
    }
    colA = checkColA;
    
    std::cout << "please type in the row of where to move, then the col of where to move" << std::endl;
    std::cin >> checkRowB;
    
    while(checkRowB<0 || checkRowB>7){
        std::cout << "you idiot... try again with a number thats in the range of 0 to 7." << std::endl;
        std::cin >> checkRowB;
    }
    rowB = checkRowB;
    

    std::cin >> checkColB;
    
    while(checkColB<0 || checkColB>7){
        std::cout << "you idiot... try again with a number thats in the range of 0 to 7." << std::endl;
        std::cin >> checkColB;
    }
    colB = checkColB;
    
}


class reinLearn{
public:
    float weights[4] = {0.5,0.5,0.5,0.5};//0 is x pc, 1 is o px, 2 is K pc, 3 is Q pc
    
    
    int calculateValue(Board game);
    void adjustWeights(Board game);
    move moveToTake(Board game);
    
    
};

int reinLearn::calculateValue(Board game){//assuming x is always rein learner..
   int value = ((game.xScor() * weights[0]) +  (game.kScor() * weights[2]))
    - ( (game.oScor() * weights[1]) +  (game.qScor() * weights[3]));
    return value;
    
}

void reinLearn::adjustWeights(Board game){
    if(game.checkIfWon()){
        
        
        if (game.xScor() < 6) {
            this->weights[0] -= .01; //this makes having lots of basic pieces not matter that much for winning, devalue holding pieces if this is the case
        }
        else{
            this->weights[0] += .01; //this means the opposite of ^
        }
        
        
        if(game.oScor() < 1){
            this->weights[1] += .03; // this will increase the value of there being less o pieces on the board at one time, essentially teaching it over time that you want to take o's pieces in order to win
        }
        if(game.qScor() < 1){
            this->weights[3] += .03; //same as above except teaching that queens need to be taken as well in order to win
        }
        
        
        if (game.kScor() > 1) {
            this->weights[2] += .01; //this makes having more than 1 king valuable
            
            if (game.kScor() > 3) {
                this->weights[2] += .04; //this makes having more than 3 kings very valuable
            }
        }

        if (game.qScor() > 3) {
            this->weights[2] += .04; //this makes having more than 3 kings very valuable
        }
        
        
    }
}


//move miniMax(Board game, int depth, int alpha, int beta){
//    if(depth == 0 || game.checkGameOver()){
//        return
//    }
//}


//int miniMaxerUnClean(Board game, int depth, int alpha, int beta, move &bestMove, bool maxOrMin){
//    if(game.checkGameOver()){
//        std::cout << "Base Case Fired from game over;" << std::endl;
//        return game.evaluateBoard();
//    }
//    else if(depth == 0){
//        std::cout << "Base Case Fired from depth limit;" << std::endl;
//        return game.evaluateBoard();
//    }
//    else if(maxOrMin){
//        int v = -10000;
//        std::vector<move> moveList = game.findAllMoves();
//        std::cout << "Move List contains"<< std::endl;
//        for(int i=0;i<moveList.size();i++){
//            
//            std::cout <<"(" << moveList.at(i).prev_x << "," << moveList.at(i).prev_y << ") to ("<< moveList.at(i).next_x << ","<< moveList.at(i).next_y <<")"<< std::endl;
//        }
//        Board placeHolder = game;
//        placeHolder.setTurn(game.whosTurn);
//
////        std::cout << "Move List contains"<< std::endl;
////        for(int i=0;i<moveList.size();i++){
////            
////            std::cout <<"(" << moveList.at(i).prev_x << "," << moveList.at(i).prev_y << ") to ("<< moveList.at(i).next_x << ","<< moveList.at(i).next_y <<")"<< std::endl;
////        }
//        for (int i = 0; i<moveList.size(); i++) {
//            placeHolder = game;
//            placeHolder.setTurn(game.whosTurn);
//            placeHolder.makeMove(moveList.at(i).prev_x, moveList.at(i).prev_y, moveList.at(i).next_x, moveList.at(i).next_y);
//            
//            
////            std::vector<move> moveList2 = placeHolder.findAllMoves();
////            std::cout << "Move List ALternate contains"<< std::endl;
////            for(int i=0;i<moveList2.size();i++){
////                
////                std::cout <<"(" << moveList2.at(i).prev_x << "," << moveList2.at(i).prev_y << ") to ("<< moveList2.at(i).next_x << ","<< moveList2.at(i).next_y <<")"<< std::endl;
////            }
//            
//            
//            int scoreAgainst =  miniMaxer(placeHolder, depth - 1, alpha, beta, bestMove, false);
//            if (v < scoreAgainst) {
//                v = scoreAgainst;
//                bestMove = moveList.at(i);
////                std::cout << "New Best Move for Max!! " << std::endl;
////                std::cout <<"(" << moveList.at(i).prev_x << "," << moveList.at(i).prev_y << ") to ("<< moveList.at(i).next_x << ","<< moveList.at(i).next_y <<")"<< std::endl;
////                std::cout << "It's score was:" << v << std::endl;
//            }
//            placeHolder = game;
//            //v = std::max(v, miniMaxer(placeHolder, depth - 1, alpha, beta, bestMove));
//            
//            
////            alpha = std::max(alpha, v);
////            if (beta<=alpha) {
////                break;
////            }
////                        
//        }
//        std::cout << "Best Move Max was: ";
//        std::cout <<"(" << bestMove.prev_x << "," << bestMove.prev_y << ") to ("<< bestMove.next_x << ","<< bestMove.next_y <<")"<< std::endl;
//        return v;
//    }
//    else{
//        int v = 10000;
//        std::vector<move> moveList = game.findAllMoves();
//
//        std::cout << "Move List contains"<< std::endl;
//        for(int i=0;i<moveList.size();i++){
//            
//            std::cout <<"(" << moveList.at(i).prev_x << "," << moveList.at(i).prev_y << ") to ("<< moveList.at(i).next_x << ","<< moveList.at(i).next_y <<")"<< std::endl;
//        }
//        Board placeHolder = game;
//        placeHolder.setTurn(game.whosTurn);
//        for (int i = 0; i<moveList.size(); i++) {
//            placeHolder = game;
//            placeHolder.setTurn(game.whosTurn);
//            placeHolder.makeMove(moveList.at(i).prev_x, moveList.at(i).prev_y, moveList.at(i).next_x, moveList.at(i).next_y);
//            int scoreAgainst =  miniMaxer(placeHolder, depth - 1, alpha, beta, bestMove, true);
//            if (v > scoreAgainst) {
//                v = scoreAgainst;
//                bestMove = moveList.at(i);
////                std::cout << "New Best Move for Max!! " << std::endl;
////                std::cout <<"(" << moveList.at(i).prev_x << "," << moveList.at(i).prev_y << ") to ("<< moveList.at(i).next_x << ","<< moveList.at(i).next_y <<")"<< std::endl;
////                std::cout << "It's score was:" << v << std::endl;
//            }
//            placeHolder = game;
//            //v = std::min(v, miniMaxer(placeHolder, depth - 1, alpha, beta));
//            
//            
////            beta = std::min(beta, v);
////            if (beta<=alpha) {
////                break;
////            }
//            
//        }
//        std::cout << "Best Move Min was: ";
//        std::cout <<"(" << bestMove.prev_x << "," << bestMove.prev_y << ") to ("<< bestMove.next_x << ","<< bestMove.next_y <<")"<< std::endl;
//        return v;
//    }
//    
//    
//}

int miniMaxer(Board game, int depth, int alpha, int beta, move &bestMove, bool maxOrMin){
    if(game.checkGameOver()){
        return game.evaluateBoard();
    }
    else if(depth == 0){

        return game.evaluateBoard();
    }
    else if(maxOrMin){
        int v = -10000;
        std::vector<move> moveList = game.findAllMoves();
        Board placeHolder = game;
        placeHolder.setTurn(game.whosTurn);
        for (int i = 0; i<moveList.size(); i++) {
            placeHolder = game;
            placeHolder.setTurn(game.whosTurn);
            placeHolder.makeMove(moveList.at(i).prev_x, moveList.at(i).prev_y, moveList.at(i).next_x, moveList.at(i).next_y);
            move otherBestMove;
            int scoreAgainst =  miniMaxer(placeHolder, depth - 1, alpha, beta, otherBestMove, false);
            if (v < scoreAgainst) {
                v = scoreAgainst;
                bestMove = moveList.at(i);
            }
            //v = std::max(v, miniMaxer(placeHolder, depth - 1, alpha, beta, bestMove));
                        alpha = std::max(alpha, v);
                        if (beta<=alpha) {
                            break;
                        }
        }
        return v;
    }
    else{
        int v = 10000;
        std::vector<move> moveList = game.findAllMoves();
        Board placeHolder = game;
        placeHolder.setTurn(game.whosTurn);
        for (int i = 0; i<moveList.size(); i++) {
            placeHolder = game;
            placeHolder.setTurn(game.whosTurn);
            placeHolder.makeMove(moveList.at(i).prev_x, moveList.at(i).prev_y, moveList.at(i).next_x, moveList.at(i).next_y);
            move otherBestMove;
            int scoreAgainst =  miniMaxer(placeHolder, depth - 1, alpha, beta, otherBestMove, true);
            if (v > scoreAgainst) {
                v = scoreAgainst;
                bestMove = moveList.at(i);
                std::cout << "It's score was:" << v << std::endl;
            }
            placeHolder = game;
            //v = std::min(v, miniMaxer(placeHolder, depth - 1, alpha, beta));
                        beta = std::min(beta, v);
                        if (beta<=alpha) {
                            break;
                        }
        }
        return v;
    }
}


int miniMaxerKindOfOld(Board game, int depth, int alpha, int beta, move &bestMove, bool maxOrMin){
    if(game.checkGameOver()){
        std::cout << "Base Case Fired from game over;" << std::endl;
        return game.evaluateBoard();
    }
    else if(depth == 0){
        std::cout << "Base Case Fired from depth limit;" << std::endl;
        return game.evaluateBoard();
    }
    else if(maxOrMin){
        int v = -10000;
        std::vector<move> moveList = game.findAllMoves();

      
        

        for (int i = 0; i<moveList.size(); i++) {
           
            game.makeMove(moveList.at(i).prev_x, moveList.at(i).prev_y, moveList.at(i).next_x, moveList.at(i).next_y);
            int scoreAgainst =  miniMaxer(game, depth - 1, alpha, beta, bestMove, false);
            if (v < scoreAgainst) {
                v = scoreAgainst;
                bestMove = moveList.at(i);
              
            }
            game.undoMove(moveList.at(i).prev_x, moveList.at(i).prev_y, moveList.at(i).next_x, moveList.at(i).next_y);

            //v = std::max(v, miniMaxer(placeHolder, depth - 1, alpha, beta, bestMove));
            
            
            //            alpha = std::max(alpha, v);
            //            if (beta<=alpha) {
            //                break;
            //            }
            //
        }
        std::cout << "Best Move Max was: ";
        std::cout <<"(" << bestMove.prev_x << "," << bestMove.prev_y << ") to ("<< bestMove.next_x << ","<< bestMove.next_y <<")"<< std::endl;
        return v;
    }
    else{
        int v = 10000;
        std::vector<move> moveList = game.findAllMoves();
        

        for (int i = 0; i<moveList.size(); i++) {

            game.makeMove(moveList.at(i).prev_x, moveList.at(i).prev_y, moveList.at(i).next_x, moveList.at(i).next_y);
            int scoreAgainst =  miniMaxer(game, depth - 1, alpha, beta, bestMove, true);
            if (v > scoreAgainst) {
                v = scoreAgainst;
                bestMove = moveList.at(i);
                //                std::cout << "New Best Move for Max!! " << std::endl;
                //                std::cout <<"(" << moveList.at(i).prev_x << "," << moveList.at(i).prev_y << ") to ("<< moveList.at(i).next_x << ","<< moveList.at(i).next_y <<")"<< std::endl;
                //                std::cout << "It's score was:" << v << std::endl;
            }
            //placeHolder = game;
            //v = std::min(v, miniMaxer(placeHolder, depth - 1, alpha, beta));
            
            
            //            beta = std::min(beta, v);
            //            if (beta<=alpha) {
            //                break;
            //            }
            
        }
        std::cout << "Best Move Min was: ";
        std::cout <<"(" << bestMove.prev_x << "," << bestMove.prev_y << ") to ("<< bestMove.next_x << ","<< bestMove.next_y <<")"<< std::endl;
        return v;
    }
    
    
}




int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    Board game;
    
    while(!game.checkGameOver()){
        std::cout << "it is currently " << game.whosTurn << "'s turn" << std::endl;
        
        std::vector<move> moveList = game.findAllMoves();
        std::cout << "Move List contains"<< std::endl;
        for(int i=0;i<moveList.size();i++){
            
            std::cout <<"(" << moveList.at(i).prev_x << "," << moveList.at(i).prev_y << ") to ("<< moveList.at(i).next_x << ","<< moveList.at(i).next_y <<")"<< std::endl;
        }
        std::cout << "it is currently " << game.whosTurn << "'s turn" << std::endl;
        std::cout << "it is currently " << game.whosTurn << "'s turn" << std::endl;
        
        
//        game.printBoard(game);
//        game.makeMove(5,1,7,3);
//        game.printBoard(game);
//        game.undoMove(5,1,7,3);
game.printBoard(game);

        move bestMove;
        miniMaxer(game,8, -10000, 10000, bestMove,  true);
        std::cout << "The best move is ("<<bestMove.prev_x<<","<<bestMove.prev_y<<") to ("<<bestMove.next_x<<","<<bestMove.next_y<<")."<<std::endl;
        rowA = bestMove.prev_x;
        colA = bestMove.prev_y;
        rowB = bestMove.next_x;
        colB = bestMove.next_y;
        game.makeMove(rowA, colA, rowB, colB);
        game.printBoard(game);
        
        
        //opponent
//        miniMaxer(game,3, -10000, 10000, bestMove,  false);
//        std::cout << "The best move is ("<<bestMove.prev_x<<","<<bestMove.prev_y<<") to ("<<bestMove.next_x<<","<<bestMove.next_y<<")."<<std::endl;
//        rowA = bestMove.prev_x;
//        colA = bestMove.prev_y;
//        rowB = bestMove.next_x;
//        colB = bestMove.next_y;
//        game.makeMove(rowA, colA, rowB, colB);
        
        game.askMove();
        game.makeMove(rowA, colA, rowB, colB);
        rowA = 0;
        colA = 0;
        rowB = 0;
        colB = 0;
//    }
//    game.printBoard(game);
//    std::cout << "Board Score:";
//    std::cout << game.evaluateBoard() << std::endl;
//    std::vector<move> moveList = game.findAllMoves();
//    std::cout << "Move List contains"<< std::endl;
//    for(int i=0;i<moveList.size();i++){
//        
//        std::cout <<"(" << moveList.at(i).prev_x << "," << moveList.at(i).prev_y << ") to ("<< moveList.at(i).next_x << ","<< moveList.at(i).next_y <<")"<< std::endl;
  }
    
    std::cout << "GAME OVER" << std::endl;
    usleep(300000);
    std::cout << "PLEASE WAIT HAL IS CALCULATING THIS IS SUPER HARD" << std::endl;
    usleep(300000);
    std::cout << "." << std::endl;
    usleep(300000);
    std::cout << ".." << std::endl;
    usleep(300000);
    std::cout << "..." << std::endl;
    usleep(300000);
    std::cout << "...." << std::endl;
    usleep(300000);
    
//    move bestMove;
//    miniMaxer(game,5, -10000, 10000, bestMove, true);
//    std::cout << "The best move is ("<<bestMove.prev_x<<","<<bestMove.prev_y<<") to ("<<bestMove.next_x<<","<<bestMove.next_y<<")."<<std::endl;
    if(game.xScore() == 0 && game.oScore() > 0){
        std::cout << "o/Q WINS" << std::endl;
    }
    else if(game.oScore() == 0 && game.xScore() > 0){
        std::cout << "x/K WINS" << std::endl;
    }
    
    

    
    

    return 0;
}


