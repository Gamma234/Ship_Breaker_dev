#ifndef GAMMA_H_INCLUDED
#define GAMMA_H_INCLUDED

#include <iostream>
#include <string>
#include <math.h>

using namespace std ;

namespace GA {

    int StringToInt( string str ) ; // 盢string锣传Θint
    string IntToString( int num ) ; // 盢int锣传Θstring

} ;

int GA::StringToInt( string str ) { // 盢string锣传Θint

	int i = 0, value = 0 ;
    
    if ( str[0] == '-' ) i = 1 ;
    else i = 0 ;

    for ( int mul = str.length() - i - 1 ; i < str.length() ; i++, mul-- )
        value = value + ( str[i] - '0' ) * pow( 10, mul ) ;

    return ( str[0] == '-' ? value * (-1) : value ) ;

} // GA::StringToInt()

string GA::IntToString( int num ) { // 盢int锣传Θstring
	
	string temp = "", temp2 = "" ;
	
	if ( num < 0 ) {
		num *= (-1) ;
		temp2 += '-' ;
	} // if	
	
	do {
		temp += (char)( num % 10 + '0' ) ;
		num /= 10 ;
	} while ( num != 0 ) ;
	
	for ( int i = temp.length() - 1 ; i >= 0 ; i-- )
		temp2 += temp[i] ;
	
	return temp2 ;
	
} // GA::IntToString()


#endif // GAMMA_H_INCLUDED
