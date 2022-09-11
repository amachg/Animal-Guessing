// Animal Guessing Game, learning 
// for console written in c++20 & CMake
// Binary tree structure implemented with vectors.
// 
// Conversion from my Univac-BASIC code of 16/12/86,
// studing Computer Science in Mathematics Department
// at Aristotelean University of Thessaloniki,
//
// by Machaerides Tassos, (LAST EDITED: September, 2022)

#include <iostream>
#include <iomanip> //quote
#include <string>
#include <vector>
#include <algorithm> //all_of

struct Element {
	std::string question_or_animal;
	struct To_next {
		size_t yes_ID, no_ID;
	} to_next;
	bool is_an_animal() const { 
		return to_next.yes_ID == 0 && to_next.no_ID == 0; 
	}
};

std::vector<Element> database { 
	{"Does it fly?", {2,1}},
	{"Does it swim?", {4,3}},
	{"Eagle", {0,0}},
	{"Dog", {0,0}},
	{"Shark", {0,0}}
};

int main() {
	do {
		std::cout << "Please think of an animal.... Press Enter key when ready.\n";
		std::cin.ignore();

		// Ask all the stored questions
		size_t current_index{ 0 };
		size_t last_question_index{ 0 };
		char typed_key{};
		char last_question_key{};
		do {
			const auto& element = database[current_index];
			const auto& question = element.question_or_animal;
			std::cout << question << '\n';
			do {
				std::cout << "Please answer Yes (y) or No (n)\n";
				std::cin >> typed_key;
			} while (!std::cin.fail() && typed_key != 'y' && typed_key != 'n');
			last_question_key = typed_key;
			last_question_index = current_index;

			current_index = (typed_key == 'y') ? element.to_next.yes_ID : element.to_next.no_ID;
			const auto& next_element = database[current_index];
			if (next_element.is_an_animal()) {
				break;	// terminal node, i.e. an animal
			}
		} while (current_index < database.size());

		// Has the animal been found?
		const auto& element = database[current_index];
		const std::string animal = element.question_or_animal;
		std::cout << "Is it a " << animal << "?\n";
		do {
			std::cout << "Please answer Yes (y) or No (n)\n";
			std::cin >> typed_key;
		} while (!std::cin.fail() && typed_key != 'y' && typed_key != 'n');

		if (typed_key == 'y') {
			std::cout << "YES, I FOUND IT !!\n";
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
		const Element new_DB_animal{ new_animal, {0,0} };
		const auto& ref = database.emplace_back(new_DB_animal);
		const size_t new_animal_index = &ref - database.data();

		// Ask new question with answer
		std::string new_question;
		do {
			new_question.clear();
			std::cout << "Now, type in a question which differentiates "
				<< std::quoted(new_animal) << " from " << std::quoted(animal) << ".\n";
			std::getline(std::cin >> std::ws, new_question);
		} while (!std::cin.fail() && new_question.empty());
		do {
			std::cout << "For the " << std::quoted(new_animal) << ", does the question "
				<< std::quoted(new_question) << " get answered with a Yes (y), or No (n) ?\n";
			std::cin >> typed_key;
		} while (!std::cin.fail() && typed_key != 'y' && typed_key != 'n');

		// Add new question to database
		const auto& new_question_indices = (typed_key == 'y') ?
			Element::To_next{ new_animal_index, current_index }: 
			Element::To_next{ current_index, new_animal_index };
		const Element new_DB_question{ new_question, new_question_indices };
		const auto& ref2 = database.emplace_back(new_DB_question);
		const size_t new_question_index = &ref2 - database.data();

		// Reconnect last question on database
		if (last_question_key == 'y')
			database[last_question_index].to_next.yes_ID = new_question_index;
		else
			database[last_question_index].to_next.no_ID = new_question_index;
	} while (true);
}