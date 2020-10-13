#ifndef GENERAL_FUNCTION_H
#define GENERAL_FUNCTION_H

#include "GeneralHeader.h"

bool is_empty_f(ifstream& pFile){
	    return pFile.peek() == std::ifstream::traits_type::eof();
}

int val_string(char* array){
	int val = 0;
	string arr_str(array, 0, 15);
	for(int i=0;i<arr_str.size();i++){
		val = val +int(arr_str[i]);
	}
	return val;
}

#endif