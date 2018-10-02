#ifndef _MYPOOLDOTH_
#define _MYPOOLDOTH_

#include <exception>
#include <set>
#include <cstring>

#include "poolex.h"

template <class T>
class pool final {
private:
	using type = std::remove_reference_t<T>;
	std::set<type*> free_pointers;
	T * place;
	size_t occupied;
	size_t amount;

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
			if (this->free_pointers.find(this->place + i) == this->free_pointers.end()) {
				(place + i)->~type();
			}
		}
		operator delete[] (this->place);
	}


	template<typename U = type>
	std::enable_if_t<std::is_array_v<U>, U*>
	alloc(U* that) {
		if (occupied < amount) {
			U* poi = *(this->free_pointers.begin());
			this->free_pointers.erase(this->free_pointers.begin());
			++occupied;
			std::memmove(poi, that, sizeof(U));
			return poi;
		}
		else {
			PoolAllocException ex;
			throw ex;
		}
	}

	template <typename U = type, typename... Args>
	std::enable_if_t<!std::is_array_v<U>, U*>  alloc(Args... args) {
		if (occupied < amount) {
			U* poi = *(this->free_pointers.begin());
			this->free_pointers.erase(this->free_pointers.begin());
			++occupied;
			new (poi) U(args...);
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

};

#endif // ! _MYPOOLDOTH_