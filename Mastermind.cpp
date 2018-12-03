#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

void set_random_seed();
int randn(int n);

struct mm_code_maker {

	void init(int i_length, int i_num) {
		length = i_length;
		num = i_num;
	}

	void generate_sequence() {
		for (int i = 0; i < length; i++) {
			sequence.push_back(randn(num));
		}
	}

	void give_feedback(const std::vector<int>& attempt, int& black_hits, int& white_hits) {
		black_hits = 0;
		white_hits = 0;
		for (int i = 0; i < sequence.size(); i++) {
			if (sequence[i] == attempt[i]) {
				black_hits = black_hits + 1;
			}
			else {
				int counter = 0;
				for (int j = 0; j < attempt.size(); j++) {
					if (sequence[i] == attempt[j] && i != j && counter == 0) {
						white_hits = white_hits + 1;
						counter = 1;
					}
				}
			}
		}
	}

	std::vector<int> sequence;

	int length;
	int num;

};


struct mm_solver {

	void init(int i_length, int i_num) {
		length = i_length;
		num = i_num;
	}

	void create_attempt(std::vector<int>& attempt) {
		if (counter1 == 0) {
			if (length < num) {
				for (int i = 0; i < length; i++) {
					attempt.push_back(i);
				}
			}
			else {
				int counter = 0;
				for (int i = 0; i < length; i++) {
					if (counter < num) {
						attempt.push_back(counter);
						counter = counter + 1;
					}
					else {
						counter = 0;
						attempt.push_back(counter);
					}
				}
			}
		}

		else {
			int temp = possiblesol[0];
			int div_temp;

			div_temp = temp / 1000;
			attempt.push_back(div_temp);
			temp = temp - div_temp * 1000;
			div_temp = temp / 100;
			attempt.push_back(div_temp);
			temp = temp - div_temp * 100;
			div_temp = temp / 10;
			attempt.push_back(div_temp);
			temp = temp - div_temp * 10;
			attempt.push_back(temp);  //store each bit of the first possible solution into attempt.
		}
	}

	void learn(std::vector<int>& attempt, int black_hits, int white_hits) {
		if (counter2 == 0) { //store all possible solutions into possiblesol at the beginning;
			std::vector<int> temp1;
			std::vector<int> temp2;
			std::vector<int> temp3;
			std::vector<int> temp4;
			for (int r = 0; r < num; r++) {
				temp1.push_back(r);
				temp2.push_back(r);
				temp3.push_back(r);
				temp4.push_back(r);
			}

			for (int a = 0; a < temp1.size(); a++) {
				for (int b = 0; b < temp2.size(); b++) {
					for (int c = 0; c < temp3.size(); c++) {
						for (int d = 0; d < temp4.size(); d++) {
							possiblesol.push_back(temp1[a] * 1000 + temp2[b] * 100 + temp3[c] * 10 + temp4[d]);
						}
					}
				}
			}
			counter2 = counter2 + 1;
		}

		std::vector<int> temp;
		int testval;
		int divide_testval;

		for (int i = 0; i < possiblesol.size(); i++) {
			std::vector<int> divide_sol;
			int temp_black = 0;
			int temp_white = 0;
			testval = possiblesol[i];

			divide_testval = testval / 1000;
			divide_sol.push_back(divide_testval);
			divide_testval = testval % 1000;
			divide_testval = divide_testval / 100;
			divide_sol.push_back(divide_testval);
			divide_testval = testval % 100;
			divide_testval = divide_testval / 10;
			divide_sol.push_back(divide_testval);
			divide_testval = testval % 10;
			divide_sol.push_back(divide_testval);  // store a integer's every bit into a vector.

			for (int j = 0; j < divide_sol.size(); j++) {
				if (divide_sol[j] == attempt[j]) {
					temp_black = temp_black + 1;
				}
				else {
					int counter = 0;
					for (int k = 0; k < attempt.size(); k++) {
						if (attempt[k] == divide_sol[j] && k != j && counter == 0) {
							temp_white = temp_white + 1;
							counter = 1;
						}
					}
				}
			} // determine the feature of possible solutions.

			if (temp_black == black_hits && temp_white == white_hits) {
				temp.push_back(testval);
			}// store values matches the feedback
		}
		possiblesol = temp;
	}

	std::vector<int> possiblesol;

	int length;
	int num;
	int counter1 = 0;
	int counter2 = 0;

};

void set_random_seed() {
	std::srand(std::time(0));
}

int randn(int n) {
	return std::rand() % n;
}