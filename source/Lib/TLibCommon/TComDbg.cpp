/* 
 * File:   TComDbg.cpp
 * Author: felsamps
 * 
 * Created on August 24, 2011, 8:18 AM
 */

#include "TComDbg.h"

FILE* TComDbg::dbgFile;
bool TComDbg::enableDbg;
int TComDbg::acum;
double TComDbg::calc_num;

TComDbg::TComDbg() {
}

void TComDbg::openDbgFile(std::string fileName) {
	if( enableDbg ) {
            if((dbgFile = fopen(fileName.c_str(), "w")) == NULL)
               printf("Error -> Dbg File could not be created\n");
	}
}

void TComDbg::closeDbgFile() {
		if( enableDbg ) {
		fclose(dbgFile);
	}
}

FILE* TComDbg::getDbgFile() {
        return dbgFile;
}


void TComDbg::printDbg(std::string className, const char* str, ...) {
	if( enableDbg ) {
		va_list args;
		char buffer[BUFSIZ];

		va_start(args, str);
		vsprintf(buffer, str, args);
		fprintf(dbgFile, "%s: %s\n", className.c_str(), buffer);
		va_end(args);
	}	
}

void TComDbg::print(const char* str, ...) {
	if( enableDbg ) {
		va_list args;
		char buffer[BUFSIZ];

		va_start(args, str);
		vsprintf(buffer, str, args);
		fprintf(dbgFile, "%s\n", buffer);
		va_end(args);
	}
}

void TComDbg::incSADCalc(){
    if(acum == 100){
        calc_num += 0.1;
        acum = 0;
    }
    else{
        acum++;
    }
}

