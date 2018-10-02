#ifndef _MYPOOLDOTH_
#define _MYPOOLDOTH_

#include <exception>
#include <set>

#include "poolex.h"

template <class T>
class pool final {
private:
	using type = std::remove_reference_t<T>;
	std::set<type*> free_pointers;
public:
	pool(size_t amount){
		this->place = static_cast<type*>(operator new[](amount * sizeof(T)));
		this->amount = amount;
		this->occupied = 0;
		for (type* poi = place; poi != place + amount; ++poi) {
			this->free_pointers.insert(poi);
		}
	}
	~pool() {
		for (size_t i = 0; i < occupied; ++i) {
			(place + i)->~type();
		}
		delete[] this->place;
	}
	type * alloc() {
		if (occupied < amount){
			type* poi = *(this->free_pointers.begin());
			this->free_pointers.erase(this->free_pointers.begin());
			++occupied;
			*poi = type();
			return poi;
		}
		else {
			PoolAllocException ex;
			throw ex;
		}
	}

	template <typename... Args>
	type * alloc(Args... args) {
		if (occupied < amount) {
			type* poi = *(this->free_pointers.begin());
			this->free_pointers.erase(this->free_pointers.begin());
			++occupied;
			*poi = type(args...);
			return poi;
		}
		else {
			PoolAllocException ex;
			throw ex;
		}
	}

	void free(type* obj) {
		if (obj >= this->place && obj < this->place + amount) {
			obj->~type();
			--occupied;
			this->free_pointers.insert(obj);
		}
		else {
			ObjOutsidePool ex;
			throw ex;
		}
	}
	type& operator [] (size_t ind) {
		return *(place + ind);
	}
private:
	T * place;
	size_t occupied;
	size_t amount;

};


















#endif // ! _MYPOOLDOTH_