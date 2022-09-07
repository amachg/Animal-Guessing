// Skepsou-Zoo.cpp : Defines the entry point for the application.
// Conversion from Univac-BASIC code AUTH/Math, 16/12/86

#include <vector>
#include "Skepsou-Zoo.h"

using namespace std;

int main()
{
	vector<string> a;
	string b;
	do {
		cout << "Σκεψου κάποιο Ζώο.... Δώσε Υ όταν είσαι έτοιμος" << endl;
		cin >> b;
	} while (b != "Y");

	int i = 1;
	int a1;
	auto& ra = a[i];
	if (i > 1)
		a1 = 1;

	return 0;
}
