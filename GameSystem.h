#ifndef GameSystem_H_INCLUDED
#define GameSystem_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Gamma.h"
#include "Map.h"
#include "Ship.h"

using namespace std ;

namespace SBG {
	
	class GameSystem {

	    private :
	    	
			SBM::Map * map ;
	    	int mapX, mapY, shipNum ;
	    	vector<SBS::Ship> shipBox ; // index is the ship number 
	
	    	vector<string> shipNameList ;
	    	vector<int> shipSizeBox ;
	    	vector<int> shipMoveAbilityBox ;
	    	vector<string> shipBreakSoundBox ;
	    	
	    	int AvailableShipNum() ;
	    	string OutputShip() ; // small(2), medium(3), big(4)
	    	void BubbleSort( vector<int> &data ) ; // small to big
	    	int SetNumber( string question, string tellStr, int min, int max ) ;
	    	void SetShip() ;
	    	void SetMap() ;
	    
	    public :
	    	
	    	GameSystem( int mapX, int mapY, int shipNum ) ; // constructor
	    	~GameSystem() { delete map ; } // destructor
	    	
	    	void Introduction() ;
	    	void Play() ;
	    	int GetNumber( string tellStr ) ; // + - 0
	    	void SetGame() ;
	    	string GetUserInput( string tellStr ) ;
	    	void Setting() ;
	    
	} ; // class GameSystem
	
} // namespace SBG

SBG::GameSystem::GameSystem( int mapX, int mapY, int shipNum ) : map( NULL ) { // constructor

    this->mapX = mapX ;
    this->mapY = mapY ;
    this->shipNum = shipNum ;

    shipNameList.push_back( "Small Ship" ) ;
    shipNameList.push_back( "Medium Ship" ) ;
    shipNameList.push_back( "Big Ship" ) ;
    shipSizeBox.push_back( 2 ) ;
    shipSizeBox.push_back( 3 ) ;
    shipSizeBox.push_back( 4 ) ;
    shipMoveAbilityBox.push_back( 2 ) ;
    shipMoveAbilityBox.push_back( 1 ) ;
    shipMoveAbilityBox.push_back( 1 ) ;
    shipBreakSoundBox.push_back( "HIT !" ) ;
    shipBreakSoundBox.push_back( "WHACK !" ) ;
    shipBreakSoundBox.push_back( "SMASH !" ) ;

    SetGame() ;

} // SBG::GameSystem::GameSystem()

void SBG::GameSystem::Introduction() {

    cout << "\n-----Ship Breaker-----" << endl ;
    cout << "The map is " << mapX << "x" << mapY << ". \nThere are " << AvailableShipNum() << " ships :" << OutputShip() << ". " << endl ;
    cout << "0. quit " << endl ;
    cout << "1. Start " << endl ;
    cout << "2. Setting " << endl ;
    cout << "-------------------" << endl ;

} // SBG::GameSystem::Introduction()

int SBG::GameSystem::AvailableShipNum() {

    int num = 0 ;

    for ( int i = 0 ; i < shipNum ; i++ ) {
        if ( shipBox[i].GetLocation().size() != 0 ) {
            num++ ;
        } // if
    } // for

    return num ;

} // SBG::GameSystem::AvailableShipNum()

string SBG::GameSystem::OutputShip() { // small(2), medium(3), big(4)

    string str = "" ;
    int availableShip = AvailableShipNum() ;

    for ( int i = 0, num = 0 ; i < shipNum ; i++ ) {
        if ( shipBox[i].GetLocation().size() != 0 ) {
            str = str + " " + shipNameList[i] + "(" + GA::IntToString( shipSizeBox[i] ) + ")" + ( num + 1 != availableShip ? "," : "" ) ;
            num++ ;
        } // if
    } // for

    return str ;

} // SBG::GameSystem::OutputShip()

void SBG::GameSystem::Play() {

    cout << "--------Play--------" << endl ;
    cout << "-1 : quit" << endl ;
    map->OutputMap( false ) ;

    int breakShip = 0, hitCondition = 0, hitPlace = 0, realShipNum = AvailableShipNum() ;
    string userInput = "", message = "" ;

    while ( realShipNum != 0 ) {

        //map->OutputMap( true ); // test
        userInput = GetUserInput( ">> Enter position : " ) ;

        if ( userInput == "-1" ) break ;

        hitCondition = map->GetHitCondition( userInput ) ; // -1 : miss // 0, 1, 2, ... : ship number

        if ( hitCondition >= 0 ) {
            hitPlace = map->GetHitPlace( userInput ) ;
            message = shipBox[hitCondition].CheckHitCondition( hitPlace ) ; // "hit", "already hit", "kill"
            if ( message == "hit" ) {
                shipBox[hitCondition].Move( *map ) ;
            } // if
            else if ( message == "kill" ) {
                breakShip++ ;
                if ( breakShip == realShipNum ) {
                    cout << "\nSuccessful !" << endl ;
                    break ;
                } // if
            } // else if
        } // if
        else {
            cout << "miss" << endl ;
        } // else

    } // while

    cout << "End\n---------------------" << endl ;

} // SBG::GameSystem::Play()

int SBG::GameSystem::GetNumber( string tellStr ) { // + - 0

	int num = 0 ;
	char trash = ' ' ;
	
	while ( true ) {
		
		cout << endl << tellStr ;
		
		cin >> num ;
		
		if ( cin.fail() ) {	// 防止輸入錯誤, 例如: 輸入字元而非整數 			
			cerr << "(Wrong number !)" << endl ;
			cin.clear() ;
			if ( num != 2147483647 && num != -2147483648 ) cin.ignore() ;
			while ( trash != '\n' ) trash = cin.get() ; // 處理掉同行"錯誤輸入"
			trash = ' ' ; // 初始化 
		} // if
		else {
			while ( trash != '\n' ) trash = cin.get() ; // 處理掉同行"其他不需要的輸入"
			trash = ' ' ; // 初始化  
			return num ;
		} // else
		
	} // while
	
} // SBG::GameSystem::GetNumber()

void SBG::GameSystem::SetGame() {
	
	if ( map != NULL ) delete map ;

    map = new SBM::Map( mapX, mapY ) ;
    shipBox.clear() ;
    vector<int> position ;

    for ( int i = 0 ; i < shipNum ; i++ ) {
        position = map->GetAvailablePosition( shipSizeBox[i] ) ; // size == 0 : input fail
        if ( position.size() == 0 ) {
            cout << "[Input Fail] " << shipNameList[i] << "(" << shipSizeBox[i] << ")" << endl ;
        } // if
        BubbleSort( position ) ;
        SBS::Ship ship( i, shipNameList[i], shipSizeBox[i], shipMoveAbilityBox[i], shipBreakSoundBox[i], position,
                      ( map->GetXLength() != 1 && position.size() != 0 && position.size() != 1 && position[1] - position[0] == 1 ? false : true ) ) ;
        map->InsertShipLocation( i, position ) ;
        shipBox.push_back( ship ) ;
    } // for

} // SBG::GameSystem::SetGame()

void SBG::GameSystem::BubbleSort( vector<int> &data ) { // small to big

    for ( int i = 0 ; i < data.size() ; i++ ) {
        for ( int j = 1 ; j < data.size() ; j++ ) {
            if ( data[j-1] > data[j] ) {
                int temp = data[j-1] ;
                data[j-1] = data[j] ;
                data[j] = temp ;
            } // if
        } // for
    } // for

} // SBG::GameSystem::BubbleSort()

string SBG::GameSystem::GetUserInput( string tellStr ) {

    string userInput = "" ;

    while ( true ) {

        cout << tellStr ;
        
        getline( cin, userInput ) ;

        if ( userInput.length() == 0 ) {
            continue ;
        } // if
        else {
            return userInput ;
        } // else

    } // while

} // SBG::GameSystem::GetUserInput()

void SBG::GameSystem::Setting() {

    while ( true ) {

        cout << "\n------Setting------" << endl ;
        cout << "0. quit setting " << endl ;
        cout << "1. Set ship " << endl ;
        cout << "2. Set map " << endl ;
        cout << "-------------------" << endl ;

        int choice = GetNumber( ">> Input a choice : " );

        if ( choice == 0 ) {
            cout << "quit setting" << endl ;
            break ;
        } // if
        else if ( choice == 1 ) {
            SetShip() ;
            break ;
        } // else if
        else if ( choice == 2 ) {
            SetMap() ;
            break ;
        } // else if
        else {
            cout << "\n[The choice doesn't exist !]\n" << endl ;
        } // else

    } // while

} // SBG::GameSystem::Setting()

int SBG::GameSystem::SetNumber( string question, string tellStr, int min, int max ) {

    int num = 0 ;

    while ( true ) {
        cout << "\n" << question << endl ;
        cout << "(Range : " << min << "~" << max << " ) ( -1 : quit )" << endl ;
        num = GetNumber( tellStr );
        if ( num == -1 ) {
            return num ;
        } // if
        if ( num < min || num > max ) {
            cout << "Out of range !" << endl ;
        } // if
        else break ;
    } // while

    return num ;

} // SBG::GameSystem::SetNumber()

void SBG::GameSystem::SetShip() {

    int shipNumTemp = 0 ;

    shipNumTemp = SetNumber( "How many ships ? ", ">> Input a number : ", 1, 26 );
    if ( shipNumTemp == -1 ) {
        cout << "\nquit !\n" << endl ;
        return ;
    } // if

    shipNum = shipNumTemp ;
    shipNameList.clear() ;
    shipSizeBox.clear() ;
    shipMoveAbilityBox.clear() ;
    shipBreakSoundBox.clear() ;

    cout << endl ;
    for ( int i = 0 ; i < shipNum ; i++ ) {
        cout << "Ship " << ( i + 1 ) << " : " << endl ;
        shipNameList.push_back( GetUserInput( ">> Name : " ) ) ;
        shipSizeBox.push_back( GetNumber( ">> Size : " ) ) ;
        shipMoveAbilityBox.push_back( GetNumber( ">> Move Ability : " ) ) ;
        shipBreakSoundBox.push_back( GetUserInput( ">> Break Sound : " ) ) ;
        cout << endl ;
    } // for

    cout << endl ;
    SetGame() ;
    cout << "\nCompleted !\n" << endl ;

} // SBG::GameSystem::SetShip()

void SBG::GameSystem::SetMap() {

    int xTemp = 0, yTemp = 0 ;

    xTemp = SetNumber( "What is the length (X) of the map ? ", ">> Input a number : ", 1, 26 ) ;
    if ( xTemp == -1 ) {
        cout << "\nquit !\n" << endl ;
        return ;
    } // if
    yTemp = SetNumber( "What is the width (Y) of the map ? ", ">> Input a number : ", 1, 26 );
    if ( yTemp == -1 ) {
        cout << "\nquit !\n" << endl ;
        return ;
    } // if

    mapX = xTemp ;
    mapY = yTemp ;

    cout << endl ;
    SetGame() ;
    cout << "\nCompleted !\n" << endl ;

} // SBG::GameSystem::SetMap()

#endif












