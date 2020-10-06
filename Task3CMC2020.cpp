#include <sstream>
#include <iostream>
#include <utility>
#include <vector>
#include <set>
#include <tuple>
#include <functional>
using namespace std;

struct Printer {
	std::string str() const{
		return ss.str();
	}
	template<typename F, typename S>
	Printer& format(std::pair<F,S> & a){
		ss << "(" << Printer().format(a.first).str() << ", " << Printer().format(a.second).str() << ")";
		return *this;
	}
	template<typename T>
	Printer& format(T a){
		ss << a;
		return *this;
	}

	template<typename T>
	Printer& format(std::vector<T> a){
		ss << "[" ;
		bool start = true;
		for(auto & elem: a){
			if (!start)
				ss << ", ";
			else start = false;
			ss << Printer().format(elem).str();
		}
		ss << " ]" ;
		return *this;
	}
	template<typename T>
	Printer& format(std::set<T> a){
		ss << "{" ;
		bool start = true;
		for(auto & elem: a){
			if (!start){
				ss << ", ";
				start = !start;
			}
			ss << Printer().format(elem).str();
		}
		ss << " }" ;
		return *this;
	}

	template <typename... Args>
	Printer& format(const std::tuple<Args...> &t) {
		int curr = 0; // костыли были необходимы, извините...(
		ss << "(" ;
		std::apply([&](const auto &... args) {
		        ((ss << ((++curr > 1) ? ", " : " ") << Printer().format(args).str()), ...);
		    }, t);
		ss << " )" ;
	  return *this;
	}
	std::stringstream ss;
};

template<typename T>
	std::string format(const T& t) {
	return Printer().format(t).str();
}

int main() {
	std::tuple<std::string, int, int> t = {"xyz", 1, 2};
	std::vector<std::pair<int, int> > v = {{1, 4}, {5, 6}};
	std::string s1 = Printer().format(" vector: ").format(v).str();
	std::cout << s1;
	// " vector: [ (1, 4), (5, 6) ]"
	std::string s2 = Printer().format(t).format(" ! ").format(0).str();
	std::cout << s2;
	// "( xyz, 1, 2 ) ! 0"
}
