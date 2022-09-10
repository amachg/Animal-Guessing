// Animal Guessing Console Game
// Conversion from Univac-BASIC code AUTH/Math, 16/12/86

#include <iostream>
#include <iomanip>	//quote
#include <string>
#include <vector>
#include <algorithm>	//all_of

struct Element {
	std::string question_or_animal;
	struct Indices {
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
	std::cin.ignore();

	// Ask all the stored questions
	size_t index{ 0 };
	char typed_key{};
	do {
		const auto& element = database[index];
		const auto& question = element.question_or_animal;
		std::cout << question << '\n';
		do {
			std::cout << "Please answer Yes (y) or No (n)\n";
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

	// Has the animal been found?
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

	// Add new animal to database
	std::string new_animal;
	do {
		new_animal.clear();
		std::cout << "Please type the name of the animal you have in your mind..\n";
		std::getline(std::cin >> std::ws, new_animal);	// can take spaces between words
	} while (!std::cin.fail() && !std::ranges::all_of(new_animal,
		[](unsigned char c) { return std::isalpha(c) || std::isblank(c); }));
	const Element new_DB_animal{ new_animal , {0,0} };
	const auto& ref = database.emplace_back(new_DB_animal);;
	const auto new_animal_index = &ref - database.data();
	std::cout << new_animal_index;

	// Add new question to database
	std::string new_question;
	do {
		new_question.clear();
		std::cout << "Now, type in a question which differentiates "
			<< std::quoted(new_animal) << " from " << std::quoted(animal) << ".\n";
		std::getline(std::cin >> std::ws, new_question);
	} while (!std::cin.fail() && new_question.empty());

	// Wire up new and last questions
	do {
		std::cout << "For the " << std::quoted(new_animal) << ", does the question "
			<< std::quoted(new_question) << " get answered with a Yes(y), or No(n)?\n";
		std::cin >> typed_key;
	} while (!std::cin.fail() && typed_key != 'y' && typed_key != 'n');
	const Element::Indices new_question_indices = (typed_key == 'y') ?
		{ new_animal_index, 0 } : { 0, new_animal_index };
	const Element new_DB_question{ new_question , new_question_indices };
	database.emplace_back(new_DB_question);
}