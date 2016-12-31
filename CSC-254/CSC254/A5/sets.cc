// CSC254 A5: Templates
// Yufei (Edward) Du & Vishnu Chittari
// sets.h

#include <string.h>
#include "sets.h"


using std::set;
using std::cout;
using std::string;



int main() {

    // Some miscellaneous code to get you started on testing your sets.
    // The following should work:

    std_simple_set<int> R;
    R += 5;
	R += 3;
    cout << "3 is " << (R.contains(3) ? "" : "not ") << "in R\n";
    cout << "5 is " << (R.contains(5) ? "" : "not ") << "in R\n";

    simple_set<double>* S = new std_simple_set<double>();
    *S += 3.14;
    cout << "pi is " << (S->contains(3.14) ? "" : "not ") << "in S\n";
    cout << "e is " << (S->contains(2.718) ? "" : "not ") << "in S\n";

    std_simple_set<string> U;
    U += "hello";
    cout << "\"hello\" is " << (U.contains("hello") ? "" : "not ") << "in U\n";
    cout << "\"foo\" is " << (U.contains("foo") ? "" : "not ") << "in U\n";

    range<string> r1("a", true, "f", true);
    cout << "\"b\" is " << (r1.contains("b") ? "" : "not ") << "in r1\n";
    cout << "\"aaa\" is " << (r1.contains("aaa") ? "" : "not ") << "in r1\n";
    cout << "\"faa\" is " << (r1.contains("faa") ? "" : "not ") << "in r1\n";

    range<const char*, lexico_less> r2("a", true, "f", true);
    cout << "\"b\" is " << (r2.contains("b") ? "" : "not ") << "in r2\n";
    cout << "\"aaa\" is " << (r2.contains("aaa") ? "" : "not ") << "in r2\n";
    cout << "\"faa\" is " << (r2.contains("faa") ? "" : "not ") << "in r2\n";


    // The following will not work correctly yet:

    range_set<int>* X = new bin_search_range_set<int>(16);
    *X += range<int>(7, true, 8, true);
    if (X->contains(4)) cout << "4 is in X\n";
    if (X->contains(5)) cout << "5 is in X\n";      // should print
    if (X->contains(6)) cout << "6 is in X\n";      // should print
    if (X->contains(7)) cout << "7 is in X\n";      // should print
    if (X->contains(8)) cout << "8 is in X\n";
    if (X->contains(9)) cout << "9 is in X\n";
	*X += range<int>(12, true, 15, false);
	*X += range<int>(21, true, 28, true);
    *X -= range<int>(6, true, 10, false);
	*X += range<int>(7, true, 7, true);
    if (X->contains(4)) cout << "4 is now in X\n";
    if (X->contains(5)) cout << "5 is now in X\n";      // should print
    if (X->contains(6)) cout << "6 is now in X\n";
    if (X->contains(7)) cout << "7 is now in X\n";
    if (X->contains(8)) cout << "8 is now in X\n";
    if (X->contains(9)) cout << "9 is now in X\n";
	if (X->contains(10)) cout << "10 is now in X\n";
	if (X->contains(11)) cout << "11 is now in X\n";
	if (X->contains(12)) cout << "12 is now in X\n";
	if (X->contains(13)) cout << "13 is now in X\n";
	if (X->contains(14)) cout << "14 is now in X\n";

    range_set<double>* Y = new bin_search_range_set<double>(16);
    *Y += range<double>(11.2, true, 23.1, true);
    if (Y->contains(14.45)) cout << "14.45 is now in Y\n";
	
	range_set<string>* X2 = new bin_search_range_set<string>(16);
    *X2 += range<string>("Aircraft", true, "Carrier", true);
    if (X2->contains("Destroyer")) cout << "Destroyer is in X2\n";
    if (X2->contains("Battleship")) cout << "Battleship is in X2\n";      // should print
    if (X2->contains("CA")) cout << "CA is in X2\n";      // should print
    if (X2->contains("Cruiser")) cout << "Cruiser is in X2\n"; 

    simple_set<double>* V = new carray_simple_set<double>('a', 'f');
	(*V)+='a';
	(*V)+='b';
	cout << "\"a\" is " << (V->contains('a') ? "" : "not ") << "in V\n";

    hashed_simple_set<int, cast_to_int<int> > H(19);
	H+=20;
	H+=15;
	cout << "\"20\" is " << (H.contains(20) ? "" : "not ") << "in H\n";
	cout << "\"15\" is " << (H.contains(15) ? "" : "not ") << "in H\n";
	cout << "\"12\" is " << (H.contains(12) ? "" : "not ") << "in H\n";
	H-=20;
	cout << "\"20\" is " << (H.contains(20) ? "" : "not ") << "in H\n";

    hashed_simple_set<int, cast_to_int<int> > W(19);
    W+=3;
    W+=22;
    cout << "\"3\" is " << (W.contains(3) ? "" : "not ") << "in W\n";
    cout << "\"22\" is " << (W.contains(22) ? "" : "not ") << "in W\n";
    cout << "\"12\" is " << (W.contains(12) ? "" : "not ") << "in W\n";

	
	//string test = "KanColle";
	//string test2 = "Fall Event";

	bin_search_simple_set<string, comp<string> > B(19);
	cout << "start inserting B\n";
	B+="wawawawahahahaha";
	B+="Kuma";
	B+="Tama";
	B+="Kitakami";
	B+="Ooi";
	cout << "finished inserting B\n";
	cout << "\"Kuma\" is " << (B.contains("Kuma") ? "" : "not ") << "in B\n";
	cout << "\"Tama\" is " << (B.contains("Tama") ? "" : "not ") << "in B\n";
	cout << "\"Wo\" is " << (B.contains("Wo") ? "" : "not ") << "in B\n";
	B-="Ooi";
	cout << "\"Ooi\" is " << (B.contains("Ooi") ? "" : "not ") << "in B\n";

    bin_search_simple_set<string, comp<string> > J(100);
    J+= "yo";
    cout << "\"yo\" is " << (J.contains("yo") ? "" : "not ") << "in J\n";
}