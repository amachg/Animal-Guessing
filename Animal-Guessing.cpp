// Animal Guessing console Game 
// with user contribution.
// 
// C++20, cmake
// Binary tree structure implemented with vectors
// Conversion from my Univac-BASIC code of 16/12/86,
// while studing Computer Science at mathematics department
// in Aristotelean university of Thessaloniki.
//
// by Machaerides Tassos, (LAST EDITED: September, 2022)

#include <fstream>
#include <iostream>
#include <iomanip> //quote
#include <string>
#include <vector>
#include <algorithm> //all_of

struct Element {
	struct Next_IDs { size_t yes_ID{}, no_ID{}; };	// zeros for animals

	std::string question_or_animal;
	Next_IDs next_IDs{};

	bool is_a_question() const { return next_IDs.yes_ID || next_IDs.no_ID; }
	friend std::istream& operator >> (std::istream& is, Element& e) {
		return is >> std::quoted(e.question_or_animal)
			>> e.next_IDs.yes_ID >> e.next_IDs.no_ID;
	}
	friend std::ostream& operator << (std::ostream& os, const Element& e) {
		return os << std::quoted(e.question_or_animal) << '\t'
			<< e.next_IDs.yes_ID << '\t' << e.next_IDs.no_ID;
	}
};

struct Database {
	std::fstream fileStream;
	std::vector<Element> elements;

	Database() {
		// Open database file.
		const std::string filename = "..//..//..//DataBase.txt";
		fileStream.open(filename);
		if (!fileStream.is_open()) {
			std::cout << "I cannot open file: DataBase.txt\n";
			std::exit(EXIT_FAILURE);
		}
		// Read all stored database.
		std::istream_iterator<Element> first(fileStream), last;
		elements.assign(first, last);
		fileStream.clear();                 // clear fail and eof bits
		std::cout << "Reading " << elements.size() << " questions..\n";
	}
	void print_file() {
		// Print file database
		std::cout << "File contents:\n";
		//std::cout << "Current pos:  " << fileStream.tellg() << std::endl;
		fileStream.seekg(0); // rewind
		std::cout << fileStream.rdbuf() << std::endl;
		//std::cout << "Current pos:  " << fileStream.tellg() << std::endl;
	}
	void print() const {
		// Print memory database
		std::cout << "Vector contents:\n";
		std::copy(elements.cbegin(), elements.cend(),
			std::ostream_iterator<Element>(std::cout, "\n"));
		std::cout << std::endl;
	}
	void update_file() {
		// Write updated database to file.
		std::cout << "Writing " << elements.size() << " database to file..\n";
		fileStream.seekg(0); // rewind to overwrite
		const auto after_last_iter = std::copy(elements.cbegin(), elements.cend(),
			std::ostream_iterator<Element>(fileStream, "\n"));
	}

	struct Last_question { size_t index{ 0 }; char responce{}; };

	std::tuple<const std::string, const size_t, const Last_question >
	ask_all_questions_until_animal() const {
		// Ask all the stored questions
		size_t current_index{ 0 };
		Element curr_element = elements.front();
		Last_question last_question;

		while (curr_element.is_a_question()) {	// and not an animal
			const auto& question = curr_element.question_or_animal;
			std::cout << question << '\n';

			char key_stroke{};
			do {
				std::cout << "Please enter y for Yes, or n for No\n";
				std::cin >> key_stroke;
			} while (!std::cin.fail() && key_stroke != 'y' && key_stroke != 'n');
			// backup last question answer..		// ..and question index to return
			last_question = Last_question{ current_index, key_stroke };

			const auto& next_IDs = curr_element.next_IDs;
			current_index = (key_stroke == 'y') ? next_IDs.yes_ID : next_IDs.no_ID;
			curr_element = elements.at(current_index);	// move to next element
		} 
		// terminal node, element is an animal
		return { curr_element.question_or_animal, current_index, last_question };
	}

	bool is_animal_in_mind(const auto& animal) const {
		// Has the animal been found?
		std::cout << "Is it a " << animal << "?\n";
		char key_stroke{};
		do {
			std::cout << "Please enter y for Yes, or n for No\n";
			std::cin >> key_stroke;
		} while (!std::cin.fail() && key_stroke != 'y' && key_stroke != 'n');

		return key_stroke == 'y';
	}

	std::tuple<const std::string, size_t> add_new_animal() {
		// Add new animal to database.elements
		std::string new_animal;
		do {
			new_animal.clear();
			std::cout << "Enter the name of the animal you have in mind.\n";
			std::getline(std::cin >> std::ws, new_animal);	// can take spaces between words
		} while (!std::cin.fail() && !std::ranges::all_of(new_animal,
			[](unsigned char c) { return std::isalpha(c) || std::isblank(c); }));
		const Element new_element{ new_animal, {0,0} };
		const auto& ref_animal = elements.emplace_back(new_element);
		const size_t new_animal_index = &ref_animal - elements.data();

		return {new_animal, new_animal_index};
	}

	auto add_new_question(const auto& last_animal, const auto& new_animal,
		const auto last_animal_index, const auto new_animal_index) {

		// Ask new question on new animal
		std::string new_question;
		do {
			new_question.clear();
			std::cout << "Now, type in a question which differentiates "
				<< std::quoted(new_animal) << " from " << std::quoted(last_animal) << ".\n";
			std::getline(std::cin >> std::ws, new_question);
		} while (!std::cin.fail() && new_question.empty());

		// Ask the answer to this question on new animal
		char key_stroke{};
		do {
			std::cout << "Please enter y for Yes, or n for No, on the question "
				<< std::quoted(new_question) << ", about the animal "
				<< std::quoted(new_animal) << ".\n";
			std::cin >> key_stroke;
		} while (!std::cin.fail() && key_stroke != 'y' && key_stroke != 'n');
		std::cin.get(); // eat the ENTER key char

		// Add user added question to database
		const auto& new_question_indices = (key_stroke == 'y') ?
			Element::Next_IDs{ new_animal_index, last_animal_index } :
			Element::Next_IDs{ last_animal_index, new_animal_index };
		const Element new_element{ new_question, new_question_indices };
		const auto& ref_quest = elements.emplace_back(new_element); // invalidates all pointers

		return &ref_quest - elements.data();
	}

	void reconnect_last_question(const auto new_question_index, 
		const auto last_question) {
		// Reconnect last stored question of database
		if (last_question.responce == 'y')
			elements.at(last_question.index).next_IDs.yes_ID = new_question_index;
		else 
			elements.at(last_question.index).next_IDs.no_ID = new_question_index;
	}
};

int main() {
	Database database;
	database.print_file();
	database.print();

	do {
		std::cout << "Please think of an animal.... Press Enter key when ready.\n";
		std::cin.ignore();

		const auto& [last_animal, last_animal_index, last_question] =
			database.ask_all_questions_until_animal();

		if (database.is_animal_in_mind(last_animal)) {
			std::cout << "YES, I FOUND IT !!\n";
			break;
		}
		const auto [new_animal, new_animal_index] = database.add_new_animal();

		const auto new_question_index = database.add_new_question(last_animal,
			new_animal, last_animal_index, new_animal_index);

		database.reconnect_last_question(new_question_index, last_question);

	} while (true);

	// Print updated database.
	database.print();
	database.update_file();
	database.print_file();
}