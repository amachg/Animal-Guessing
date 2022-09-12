// Animal Guessing Game
// for consoles written in c++20 & CMake
// Binary tree structure implemented with vectors.
// Users contributing in questions database.elements.
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
#include <fstream>

struct To_next {
	size_t yes_ID, no_ID;
};

struct Element {
	std::string question_or_animal;
	To_next to_next;
	bool is_an_animal() const {
		return to_next.yes_ID == 0 && to_next.no_ID == 0;
	}
	friend std::istream& operator >> (std::istream& is, Element& e) {
		return is >> std::quoted(e.question_or_animal)
			>> e.to_next.yes_ID >> e.to_next.no_ID;
	}
	friend std::ostream& operator << (std::ostream& os, const Element& e) {
		return os << std::quoted(e.question_or_animal) << '\t'
			<< e.to_next.yes_ID << '\t' << e.to_next.no_ID;
	}
};

struct Database {
	std::fstream fileStream;
	std::vector<Element> elements;

	size_t current_index{};
	size_t last_question_index{};
	char key_stroke{};
	char last_question_key{};

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
	void overwrite_file_with_updated_DB() {
		// Write updated database to file.
		std::cout << "Writing " << elements.size() << " database to file..\n";
		fileStream.seekg(0); // rewind to overwrite
		const auto after_last_iter = std::copy(elements.cbegin(), elements.cend(),
			std::ostream_iterator<Element>(fileStream, "\n"));
	}
	void print_file() {
		// Print file database
		std::cout << "File contents:\n";
		//std::cout << "Current pos:  " << fileStream.tellg() << std::endl;
		fileStream.seekg(0); // rewind
		std::cout << fileStream.rdbuf() << std::endl;
		//std::cout << "Current pos:  " << fileStream.tellg() << std::endl;
	}
	void print() {
		// Print memory database
		std::cout << "Vector contents:\n";
		std::copy(elements.cbegin(), elements.cend(),
			std::ostream_iterator<Element>(std::cout, "\n"));
		std::cout << std::endl;
	}
	auto& ask_all_questions_until_animal() {
		// Ask all the stored questions
		current_index = 0;
		Element* element = &elements[current_index];
		do {
			const auto& question = element->question_or_animal;
			std::cout << question << '\n';
			do {
				std::cout << "Please enter y for Yes, or n for No\n";
				std::cin >> key_stroke;
			} while (!std::cin.fail() && key_stroke != 'y' && key_stroke != 'n');
			last_question_key = key_stroke;
			last_question_index = current_index;

			current_index = (key_stroke == 'y') ?
				element->to_next.yes_ID :
				element->to_next.no_ID;
			element = &elements[current_index];
		} while (!element->is_an_animal());	// terminal node, i.e. an animal

		return element->question_or_animal;
	}
	bool is_animal_found(const auto& animal) {
		// Has the animal been found?
		std::cout << "Is it a " << animal << "?\n";
		do {
			std::cout << "Please enter y for Yes, or n for No\n";
			std::cin >> key_stroke;
		} while (!std::cin.fail() && key_stroke != 'y' && key_stroke != 'n');

		if (key_stroke == 'y') {
			std::cout << "YES, I FOUND IT !!\n";
			return true;
		}
		else
			return false;
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
		const Element new_DB_animal{ new_animal, {0,0} };
		const auto& ref = elements.emplace_back(new_DB_animal);
		const size_t new_animal_index = &ref - elements.data();
		return {new_animal, new_animal_index};
	}
	auto ask_new_question(const auto& animal, const auto& new_animal) {
		// Ask new question with answer
		std::string new_question;
		do {
			new_question.clear();
			std::cout << "Now, type in a question which differentiates "
				<< std::quoted(new_animal) << " from " << std::quoted(animal) << ".\n";
			std::getline(std::cin >> std::ws, new_question);
		} while (!std::cin.fail() && new_question.empty());
		do {
			std::cout << "Please enter y for Yes, or n for No, on the question "
				<< std::quoted(new_question) << ", about the animal "
				<< std::quoted(new_animal) << ".\n";
			std::cin >> key_stroke;
		} while (!std::cin.fail() && key_stroke != 'y' && key_stroke != 'n');
		std::cin.get(); // eat the ENTER

		return new_question;
	}
	void add_new_question(const auto& new_question, auto new_animal_index) {
		// Add new question to database
		const auto& new_question_indices = (key_stroke == 'y') ?
			To_next{ new_animal_index, current_index } :
			To_next{ current_index, new_animal_index };
		const Element new_DB_question{ new_question, new_question_indices };
		const auto& ref2 = elements.emplace_back(new_DB_question);
		const size_t new_question_index = &ref2 - elements.data();

		// Reconnect last question on database
		if (last_question_key == 'y')
			elements[last_question_index].to_next.yes_ID = new_question_index;
		else
			elements[last_question_index].to_next.no_ID = new_question_index;
	}
};

int main() {
	Database database;
	database.print_file();
	database.print();

	do {
		std::cout << "Please think of an animal.... Press Enter key when ready.\n";
		std::cin.ignore();

		const auto& animal = database.ask_all_questions_until_animal();

		if(database.is_animal_found(animal))
			break;

		const auto [new_animal, new_animal_index] = database.add_new_animal();

		const auto& new_question = database.ask_new_question(animal, new_animal);

		database.add_new_question(new_question, new_animal_index);

	} while (true);

	// Print updated database.
	database.print();
	database.overwrite_file_with_updated_DB();
	database.print_file();
}