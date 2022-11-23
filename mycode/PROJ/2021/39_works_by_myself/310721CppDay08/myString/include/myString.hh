#include <string.h>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class String {
    #define MAX_INPUT_BUFFER 1024*2
public:
    /* String(); */
    /* String(const char *); */
    String(const char *lhs = nullptr);
    String(const String &);
    ~String();
    String &operator=(const String &);
    String &operator=(const char *);

    String &operator+=(const String &);
    String &operator+=(const char *);

    char &operator[](std::size_t index);
    const char &operator[](std::size_t index) const;

    std::size_t size() const;
    const char* c_str() const;

    friend bool operator==(const String &, const String &);
    friend bool operator!=(const String &, const String &);

    friend bool operator<(const String &, const String &);
    friend bool operator>(const String &, const String &);
    friend bool operator<=(const String &, const String &);
    friend bool operator>=(const String &, const String &);

    friend std::ostream &operator<<(std::ostream &os, const String &s);
    friend std::istream &operator>>(std::istream &is, String &s);

    friend String operator+(const String &lhs, const String &rhs);
    friend String operator+(const String &lhs, const char *rhs);
    friend String operator+(const char *lhs, const String &rhs);

private:
    char * _pstr;

};

String operator+(const String &, const String &);
String operator+(const String &, const char *);
String operator+(const char *, const String &);
