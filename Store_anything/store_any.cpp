#include <iostream>
using namespace std;


class Base{
public:
	virtual void print() const = 0;
	virtual ~Base(){};
};

template<typename T>
class Child:public Base{
public:
	Child(T datainp):data(datainp){};
	void print() const{
		cout << data << '\n';
	}
private:
	T data;
};

template<typename T>
class Child_ptr:public Base{
public:
	Child_ptr(T *datainp):data(datainp){};
	void print() const{
		cout << '[' <<*data << ']' << '\n';
	}
private:
	T *data;
};

class Printer{
public:
	Printer():data(NULL){};
	void print() const{
		data->print();
	}

	template<typename T>
	Printer & operator=(T x){
		delete data;
		inp(static_cast<T>(x));
		return *this;
	}
private:
	template<typename T>
	void inp(T * d){
		data = new Child_ptr<T>(d);
	}

	template<typename T>
	void inp(T && d){
		data = new Child<T>(d);
	}
	Base * data;
};

int main() {
	Printer printer;

	printer = 42;
	printer.print(); //should print "42" to standard output

	int* value = new int(10);
	printer = value;
	printer.print(); // should print "[10]"

	*value = 20; // the value pointer did not change, changed a value by it
	printer.print(); // should print "[20]"

	float* fvalue = new float(9.81);
	printer = fvalue;
	printer.print(); // should print "[9.81]"

    *fvalue = 0.2+0.3;
	printer.print(); // should print "[0.5]"

	printer = std::string("Hello world");
	printer.print();
	//should print "Hello world"

	printer = 2.718281;
	printer.print();
	//should print "2.718281"

    delete fvalue;
    delete value;
	return 0;
}
