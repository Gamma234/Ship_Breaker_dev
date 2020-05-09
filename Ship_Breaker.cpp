#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <math.h>
#include <iomanip>

#include "GameSystem.h"

using namespace std ;

int main() {
	
    SBG::GameSystem * game = new SBG::GameSystem( 7, 7, 3 );

    int command = 0;

    while (true) {

        game->Introduction();
        command = game->GetNumber( ">> Input a command : " );

        if (command == 0) {
            cout << "\nQuit" << endl ;
            break ;
        } // if
        else if (command == 1) {
            game->Play();
            cout << endl ;
            game->SetGame();
        } // else if
        else if (command == 2) {
            game->Setting();
        } // else if
        else {
            cout << "\n[The command doesn't exist !]\n" << endl ;
        } // else

    } // while
    
    delete game ;
	
} // main()












