#ifndef _MYPOOLDOTH_
#define _MYPOOLDOTH_

#include <exception>

#include "poolex.h"

template <class T>
class pool final {
public:
	pool(size_t amount){
		this->place = static_cast<T*>(operator new[](amount * sizeof(T)));
		this->amount = amount;
		this->occupied = 0;
	}
	~pool() {
		for (size_t i = 0; i < occupied; ++i) {
			(place + occupied)->~T();
		}
		delete[] this->place;
	}
	std::remove_reference_t<T> * alloc() {
		if (occupied < amount){
			*(this->place + occupied) = T();
			++occupied;
			return place + occupied - 1;
		}
		else {
			PoolAllocException ex;
			throw ex;
		}
	}

	template <class... Args>
	std::remove_reference_t<T> * alloc(Args&&... args) {
		if (occupied < amount) {
			*(this->place + occupied) = T(args...);
			++occupied;
			return place + occupied - 1;
		}
		else {
			PoolAllocException ex;
			throw ex;
		}
	}

	void free(T* obj) {
		if (obj >= this->place && obj < this->place + amount) {
			if (obj < this->place + occupied) {
				(*obj).~T();
				for (T* cur = obj + 1; cur != this->place + occupied; ++cur) {
					*(cur - 1) = *cur;
				}
				--occupied;
			}
			// I could throw exception in this case also, but i think who cares right? Let the user free what's free alreaady.
		}
		else {
			ObjOutsidePool ex;
			throw ex;
		}
	}
	std::remove_reference_t<T> & operator [] (size_t ind) {
		return *(place + ind);
	}
private:
	T * place;
	size_t occupied;
	size_t amount;
};


















#endif // ! _MYPOOLDOTH_