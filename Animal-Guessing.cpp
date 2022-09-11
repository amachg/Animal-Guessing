﻿// Animal Guessing Console Game
// Conversion from Univac-BASIC code AUTH/Math, 16/12/86

#include <iostream>
#include <iomanip>	//quote
#include <string>
#include <vector>
#include <algorithm>	//all_of

struct Element {
	std::string question_or_animal;
	struct Answ_IDs {
		size_t yes, no;
	} answ_IDs;
	bool is_an_animal() const { return answ_IDs.yes == 0 && answ_IDs.no == 0; }
};

std::vector<Element> database { 
	{"Does it fly?", {1,2}},
	{"bird", {0,0}},
	{"fish", {0,0}}
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

			current_index = (typed_key == 'y') ? element.answ_IDs.yes : element.answ_IDs.no;
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
		const Element new_DB_animal{ new_animal, {0,0} };
		const auto& ref = database.emplace_back(new_DB_animal);
		const size_t new_animal_index = &ref - database.data();

		// Add new question to database. Wire up new and last questions.
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
		const auto& new_question_indices = (typed_key == 'y') ?
			Element::Answ_IDs{ new_animal_index, current_index }
		: Element::Answ_IDs{ current_index, new_animal_index };
		const Element new_DB_question{ new_question, new_question_indices };
		const auto& ref2 = database.emplace_back(new_DB_question);
		const size_t new_question_index = &ref2 - database.data();

		if (last_question_key == 'y')
			database[last_question_index].answ_IDs.yes = new_question_index;
		else
			database[last_question_index].answ_IDs.no = new_question_index;
	} while (true);
}