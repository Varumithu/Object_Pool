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
	using element = std::remove_all_extents_t<type>;
	type * place;
	size_t occupied;
	size_t amount;
	std::vector<bool> isfree;

	template <typename... Args>
	void array_copy(size_t al_ind, type * that, Args&&... args) {
		std::memmove(place + al_ind, that, sizeof(type));
	}

	template <typename... Args>
	void array_copy(size_t al_ind, type&& that, Args&&... args) {
		size_t i = 0;
		for (size_t i = 0; i < sizeof(type) / sizeof(element); ++i) {
			element* c = reinterpret_cast<element*>(place + al_ind) + i;
			new (c) element(*(reinterpret_cast<element*>(that) + i));
		}
	}

	void array_copy(size_t al_ind) {
		size_t i = 0;
		for (size_t i = 0; i < sizeof(type) / sizeof(element); ++i) {
			element* c = reinterpret_cast<element*>(place + al_ind) + i;
			new (c) element();
		}
	}

public:
	pool(size_t amount) : amount(amount) {
		place = static_cast<type*>(operator new[](amount * sizeof(T)));
		occupied = 0;
		isfree = std::vector<bool>(amount, true);

	}
	//pool() : amount(0), place(nullptr) {}
	~pool() {
		for (size_t i = 0; i < amount; ++i) {
			if (isfree[i] == false) {
				if constexpr (!std::is_array_v<type>) {
					(place + i)->~type();
				}
				else {
					type tt;
					element te;
					for (size_t j = 0; j < sizeof(tt) / sizeof(te); ++j) {
						element* c = reinterpret_cast<element*>(place + i) + j;
						c->~element();
					}
				}
			}
		}
		operator delete[](this->place);
	}

public:
	template <typename... Args>
	type* alloc(Args&&... args) {
		if (occupied < amount) {
			size_t al_ind;
			for (size_t i = 0; i < amount; ++i) {
				if (isfree[i] == true) {
					al_ind = i;
					break;
				}
			}
			isfree[al_ind] = false;
			++occupied;
			if constexpr (std::is_array_v<type>) {
				array_copy(al_ind, std::forward<Args>(args)...);
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
		size_t index;
		if (obj < place + amount && obj >= place) {
			index = std::distance(place, obj);
			if (isfree[index] == false) {
				if constexpr (!std::is_array_v<type>) {
					obj->~type();
					--occupied;
					isfree[index] = true;

				}
				else {
					type tt;
					element te;
					for (size_t i = 0; i < sizeof(tt) / sizeof(te); ++i) {
						element* c = reinterpret_cast<element*>(obj) + i;
						c->~element();
					}
					isfree[index] = true;
					--occupied;
				}
			}
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