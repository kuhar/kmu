/*
* macros.h
*
*  Created on: Nov 26, 2014
*      Author: Jakub 'kuhar' Kuderski
*
*/ 

#define enforce(expression, message) (void) \
	(!!(expression) || (throw std::runtime_error(message), false))