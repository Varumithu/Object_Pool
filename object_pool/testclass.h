#ifndef _MYTESTINGCLASSDOTH_
#define _MYTESTINGCLASSDOTH_


class point final {
public:
	int x, y;
	point() {
		x = 0;
		y = 0;
	}
	point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

class stupidString final {
public:
	char * str;
	stupidString(const char * str);
	stupidString(const stupidString& that); // copy
	stupidString(stupidString&& that);//move
	~stupidString();
	void set(const char* str);
	size_t length;
	stupidString& operator= (const stupidString& that);
};



#endif