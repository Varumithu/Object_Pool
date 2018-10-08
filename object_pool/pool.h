#ifndef _MYPOOLDOTH_
#define _MYPOOLDOTH_

#include <exception>
#include <cstring>
#include <vector>
#include <iterator>
#include <type_traits>

#include "poolex.h"

template <class T>
class pool final {
private:
	using type = std::remove_reference_t<T>;
	type * place;
	size_t occupied;
	size_t amount;
	std::vector<bool> isfree;

	template <typename... Args>
	void array_copy(size_t al_ind, type * that, Args... args) {
		std::memmove(place + al_ind, that, sizeof(type));
	}

public:
	pool(size_t amount) {
		this->place = static_cast<type*>(operator new[](amount * sizeof(T)));
		this->amount = amount;
		this->occupied = 0;
		for (size_t i = 0; i < amount; ++i) {
			this->isfree.emplace_back(true);
		}
	}
	~pool() {
		for (size_t i = 0; i < occupied; ++i) {
			if (isfree[i] == false) {
				(place + i)->~type();
			}
		}
		operator delete[](this->place);
	}

public:
	template <typename... Args>
	type* alloc(Args... args) {
		if (occupied < amount) {
			size_t al_ind;
			for (size_t i = 0; i < amount; ++i) {
				if (isfree[i]) {
					al_ind = i;
					break;
				}
			}
			isfree[al_ind] = false;
			++occupied;
			if constexpr (std::is_array_v<type>) {
				array_copy(al_ind, args...);
			}
			else {
				new (place + al_ind) type(std::forward<Args>(args)...);
			}
			return place + al_ind;
		}
		else {
			throw PoolAllocException();
		}
	}


	void free(type* obj) {
		if ((obj < place + amount && obj >= place) && isfree[std::distance(place, obj)] == false) {
			obj->~type();
			--occupied;
			isfree[std::distance(place, obj)] = true;
		}
		else {
			
			throw ObjOutsidePool();
		}
	}
	type& operator [] (size_t ind) {
		return *(place + ind);
	}

};

#endif // ! _MYPOOLDOTH_