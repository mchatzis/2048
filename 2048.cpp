// 2048 game on Terminal 
// made by Michail Chatzis
// all copyrights received 

#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<vector>
#include<cstdlib>
#include<cmath>
using namespace std;

struct matrixentry {
	int x;
	int y;
};
void printing(const vector <vector<int> >& pout);
bool game(bool d1, int d2, vector<vector<int> >& out);
bool game_over(const vector<vector<int> >& gout);
void rotating(vector <vector<int> >& vout);
bool push(vector <vector<int> >& veout, int dir);
void random(vector <vector<int> >& rout);

int main() {

	string ifile, udlr;
	std::cout << "Give me a correct input file for starting point or nothing for empty initial configuration! " << endl;
	cin >> ifile;
	vector <vector <int> > matrix;
	
	//Input file setup
	ifstream input;
	vector<int> vtmp;
	int itmp = 0, modulus, zero_counter = 0;

	input.open(ifile.c_str());
	bool valid_input_file = input.is_open();
	if (valid_input_file) {
                for (int k = 0; k < 4; k++) {
			for (int i = 0; i < 4; i++) {
				input >> itmp;
				modulus = itmp % 2;
				if (modulus != 0) {
					cout << "Input file contains numbers non-multiples of 2, hence the game will start from initial configuration" << endl;
					valid_input_file = false;
				}
				if (itmp == 0) {
					zero_counter++;
				}
				vtmp.push_back(itmp);
			}
			matrix.push_back(vtmp);
			vtmp.clear();
		}
		if (zero_counter == 16) {
			cout << "Game cannot work if all entries are zero, hence it will start from initial configuration!" << endl;
			matrix[3][3] = 2;
		}
	}
	if (!valid_input_file) {
		std::cout << "Wrong file or directory! Starting from initial configuration..." << endl;
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < 4; i++) {
				vtmp.push_back(itmp);
			}
			matrix.push_back(vtmp);
			vtmp.clear();
		}
		matrix[3][3] = 2;
	}
	
	bool up_down, valid_move, gameOVER;
	int direction2;
	printing(matrix);
	gameOVER = game_over(matrix);
		
	//main while loop of game
	while (!gameOVER) {
		
		cin >> udlr;
		if ((udlr == "w") || (udlr == "W")) {
			up_down = true;
			direction2 = -1;
			valid_move = game(up_down, direction2, matrix);
		}
		if ((udlr == "s") || (udlr == "S")) {
			up_down = true;
			direction2 = 1;
			valid_move = game(up_down, direction2, matrix);
		}
		if ((udlr == "a") || (udlr == "A")) {
			up_down = false;
			direction2 = -1;
			valid_move = game(up_down, direction2, matrix);
		}
		if ((udlr == "d") || (udlr == "D")) {
			up_down = false;
			direction2 = 1;	
			valid_move = game(up_down, direction2, matrix);
		}
		if (valid_move) {
			printing(matrix);
		}
		gameOVER = game_over(matrix);
	}
	std::cout << "Game over!" << endl;
	return 0;
}
void printing(const vector <vector<int> >& pout) {
        int digits_previous_number;
	for (int k = 0; k < pout.size(); k++) {
		for (int b = 0; b < pout.size(); b++) {
		        if((b - 1) >= 0){
		        digits_previous_number = log10(pout[k][b - 1] + 1);     // +1 prevents the program from crashing since log10(0) is not defined.
		                for(int a = 8 - digits_previous_number; a >= 0; a--){
		                std::cout << " "; 
		                }                                               //prints gaps between columns so as the game matrix to be easily readable
		        }		        
			std::cout << pout[k][b] << " ";
		}
		std::cout << endl;
	}
	cout << endl;
}
void rotating(vector <vector<int> >& vout) {
	int counter = 0, tmp;
	for (int i = 0; i < (vout.size() -1); i++) {
		for (int j = 1 + counter; j < vout.size(); j++) {
			tmp = vout[i][j];
			vout[i][j] = vout[j][i];
			vout[j][i] = tmp;
		}
		counter++;
	}
}
bool push(vector <vector<int> >& veout, int dir) {
	bool move = false;
	for(int a = 0; a < veout.size() - 1; a++) {
		for (int i = 0; i < veout.size(); i++) {
			for (int j = 0; j < veout.size(); j++) {
				if (((j + dir) >= 0) && ((j + dir) < veout.size())) {
					if (veout[i][j + dir] == 0 && !(veout[i][j] == 0)) {
							veout[i][j + dir] = veout[i][j];
							veout[i][j] = 0;
							move = true;
					}
				}
			}
		}
	}

return move;
}
void random(vector <vector<int> >& rout) {
	int random;
	vector<matrixentry> zeroentry;
	matrixentry tmp;
	for (int i = 0; i < rout.size(); i++) {
		for (int j = 0; j < rout.size(); j++) {
			if (rout[i][j] == 0) {
				tmp.x = j;
				tmp.y = i;
				zeroentry.push_back(tmp);
			}
		}
	}
	if(zeroentry.size() != 0){
	        random = rand() % zeroentry.size();
	        rout[zeroentry[random].y][zeroentry[random].x] = 2;             
        }
}
bool game(bool direction, int d, vector<vector<int> >& out) {
	bool move_exists = false;
	if (direction) {
		rotating(out);                  //rotate matrix if user selected up or down    
	}
	move_exists = push(out, d);             //push everything to the direction desired
		for (int i = 0; i < out.size(); i++) {
			for (int j = 0; j < out.size(); j++) {
				if (((j - 1) >= 0) && ((j - 1) < out.size())) {
					if (out[i][j - 1] == out[i][j] && out[i][j] != 0) {
						out[i][j - 1] += out[i][j];
						out[i][j] = 0;
						move_exists = true;	
					}
				}
			}
		}
	push(out, d);                   //push again everything to the direction desired
	if (direction) {
	rotating(out);                  //re-rotate to bring to normal matrix configuration
	}
	if (move_exists) {
		random(out);            //add a random 2
	}
	return move_exists;
}
bool game_over(const vector<vector<int> >& gout) {
	bool game_is_over = true;
	for (int i = 0; i < gout.size(); i++) {
		for (int j = 0; j < gout.size(); j++) {
			if (gout[i][j] == 0) {
				game_is_over = false;                           //check if any zero exists
			}
			if( ((j + 1) < gout.size()) && (gout[i][j] == gout[i][j + 1])){
			game_is_over = false;                                           //check if any two numbers are equal horizontally
			}
			if( ((i + 1) < gout.size()) && (gout[i][j] == gout[i + 1][j])){
			game_is_over = false;                                           //check if any two numbers are equal vertically
			}
		}
	}
	return game_is_over;
}
