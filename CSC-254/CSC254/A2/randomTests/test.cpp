#include <iostream>
using namespace std;
char* exceptionTest();

std::string string1 = "Wololololololololololololo\n";


int main(){
	std::string troll = string1;
	string1 = "Screw you\n";
	cout << troll;
	return 0;
}
