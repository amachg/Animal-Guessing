// Animal Guessing Console Game
// Conversion from Univac-BASIC code AUTH/Math, 16/12/86

#include <iostream>
#include <iomanip>	//quote
#include <string>
#include <vector>
#include <algorithm>	//all_of
//#include <regex>

struct Element {
	std::string question_or_animal;
	struct {
		size_t yes, no;
	} indices;
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

	size_t index{ 0 };
	char typed_key{};
	do {
		const auto& element = database[index];
		const auto& question = element.question_or_animal;
		std::cout << question << '\n';
		do {
			std::cout << "Please answer Yes (y) or No (n)\n"; 
			//Πάτα Y αν συμφωνείς ή Ν αν διαφωνείς.
			std::cin >> typed_key;
		} while (!std::cin.fail() && typed_key != 'y' && typed_key != 'n');

		index = (typed_key == 'y') ?
			element.indices.yes :
			element.indices.no;
		const auto& indices = database[index].indices;
		if (indices.yes == 0 && indices.no == 0) {
			break;	// terminal node, i.e. an animal
		}
	} while (index < database.size());

	const auto& element = database[index];
	const std::string animal = element.question_or_animal;
	std::cout << "Is it a " << animal << "?\n";
	do {
		std::cout << "Please answer Yes (y) or No (n)\n";
		std::cin >> typed_key;
	} while (!std::cin.fail() && typed_key != 'y' && typed_key != 'n');

	if (typed_key == 'y') {
		std::cout << "I HAVE FOUND IT. HOORA!\n";
		return EXIT_SUCCESS;
	}

	std::string new_animal;
	//const std::regex words("\\w+");
	do {
		new_animal.clear();
		std::cout << "Please type the name of the animal you have in your mind..\n";
		std::getline(std::cin >> std::ws, new_animal);	// can take spaces between words
		//std::cout << "getline returns: \t" << std::quoted(new_animal) << '\n';
	} while ( !std::cin.fail() && !std::ranges::all_of(new_animal,
		[](unsigned char c) { return std::isalpha(c)  || std::isblank(c); })
		//!std::regex_match(new_animal, words)
		);
	const Element new_element{ new_animal , {0,0} };
	database.emplace_back(new_element);

	std::string new_question;
	do {
		new_question.clear();		
		std::cout << "Now, type in a question which differentiates "
			<< std::quoted(new_animal) << " from " << std::quoted(animal) << ".\n";
		std::getline(std::cin >> std::ws, new_question);
	} while (!std::cin.fail() && new_question.empty());

	do {
		std::cout << "For the " << std::quoted(new_animal) << ", does the question "
			<< std::quoted(new_question) << " get answered with a Yes(y), or No(n)?\n";
		std::cin >> typed_key;
	} while (!std::cin.fail() && typed_key != 'y' && typed_key != 'n');

	const auto new_question_index = (typed_key == 'y') ?

		element.indices.yes :
		element.indices.no;

}