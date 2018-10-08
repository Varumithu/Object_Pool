#ifndef _MYPOOLEXDOTH_
#define _MYPOOLEXDOTH_

#include <exception>
#include <iostream>

class ObjectPoolException : std::exception {
	virtual void printex(std::ostream& os) {
		os << "ObjPoolException(some)";
	}

};

class PoolAllocException : ObjectPoolException {
	virtual void printex(std::ostream& os) {
		os << "Could not allocate";
	}

};

class ObjOutsidePool : ObjectPoolException {
	virtual void printex(std::ostream& os) {
		os << "You tried to free something that is outside pool";
	}

};

#endif