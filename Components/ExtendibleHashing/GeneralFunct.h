#ifndef GENERAL_FUNCTION_H
#define GENERAL_FUNCTION_H

#include "GeneralHeader.h"

bool is_empty_f(ifstream& pFile){
	    return pFile.peek() == std::ifstream::traits_type::eof();
}

#endif