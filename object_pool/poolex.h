#ifndef _MYPOOLEXDOTH_
#define _MYPOOLEXDOTH_

#include <exception>

class ObjectPoolException : std::exception {

};

class PoolAllocException : ObjectPoolException {

};

class ObjOutsidePool : ObjectPoolException {

};

#endif