// Animal Guessing Console Game
// Conversion from Univac-BASIC code AUTH/Math, 16/12/86

#include <iostream>
#include <string>
#include <vector>

struct Answers_indices {
	size_t yes_index, no_index;
};

struct Element {
	std::string question_or_animal;
	Answers_indices indices;
};

std::vector<Element> database { 
	{"Does it fly?", {1,2}},
	{"bird", {0,0}},
	{"fish", {0,0}}
};

int main() {
	std::cout << "Please think of an animal.... Press Enter key when ready.\n";
	//std::cout << "Σκέψου κάποιο Ζώο.... Πάτα το πλήκτρο Enter όταν είσαι έτοιμος\n";
	std::cin.ignore();

	int index{ 0 };
	char typed_key;
	do {
		Element& element = database[index];
		const auto question = element.question_or_animal;
		std::cout << question << '\n';
		do {
			std::cout << "Please answer Yes (y) or No (n)\n"; //Πάτα Y αν συμφωνείς ή Ν αν διαφωνείς.
			std::cin >> typed_key;
		} while (!std::cin.fail() && typed_key != 'y' && typed_key != 'n');

		const auto index = (typed_key == 'y') ?
			element.indices.yes_index :
			element.indices.no_index;
		const auto indices = database[index].indices;
		if (indices.yes_index == 0 && indices.no_index == 0) {
			break;	// terminal node, i.e. an animal
		}
	} while (index < database.size());

	Element& element = database[index];
	const auto animal = element.question_or_animal;
	std::cout << "Is it a(n)" << animal << "?\n";
	do {
		std::cout << "\nPlease answer Yes (y) or No (n)\n"; //Πάτα Y αν συμφωνείς ή Ν αν διαφωνείς.
		std::cin >> typed_key;
	} while (!std::cin.fail() && typed_key != 'y' && typed_key != 'n');

	if (typed_key == 'y')
		std::cout << "I HAVE FOUND IT. HOORA!\n";
	else {
		std::cout << "\nPlease answer Yes (y) or No (n)\n"; //Πάτα Y αν συμφωνείς ή Ν αν διαφωνείς.
		std::cin >> typed_key;

	}
}
