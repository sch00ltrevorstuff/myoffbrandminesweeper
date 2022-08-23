#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

class gridsq {
public:
	int x, y, minenum;
	bool cleared, hasmine, hasflag;
	//Prints the grid square status
	void displaysq() {
		if (hasflag) {
			cout << "F  ";
		}
		else if (cleared == false) {
			cout << "?  ";
		}
		else if (minenum == 0) {
			cout << "   ";
		}
		else {
			cout << minenum << "  ";
		}
	}
	//If selected then checks if has mine. If has mine then returns false, otherwise true.
	bool clearing() {
		if (hasmine == true) {
			return false;
		}
		else {
			return true;
		}
	}
 };

int main(void) {
	const int xdim=10;
	const int ydim=10;
	const int totalmines=5;
	cout << "Win by clearing all areas that don't have mines." << endl << endl;
	//Creates the grid
	vector<vector<gridsq>> gridsqs(xdim);
	for (int i = 0; i < xdim; i++) {
		for (int j = 0; j < ydim; j++) {
			gridsq newgridsq;
			newgridsq.x = i;
			newgridsq.y = j;
			newgridsq.cleared = false;
			newgridsq.hasflag = false;
			newgridsq.hasmine = false;
			newgridsq.minenum = 0;
			gridsqs[i].push_back(newgridsq);
		}
	}
	//Creates mines
	srand (time(0));
	vector<vector<int>> minelocs(xdim); //Locations of mines
	int xrand, yrand;
	bool loctaken; //For checking if there is a mine there already or not, assume it is taken.
	for (int i = 0; i < totalmines; i++) {
		loctaken = true; //For checking if there is a mine there already or not, assume it is taken.
		//Keep trying to make a new mine location until it finds a spot that doesn't have one already.
		while (loctaken) {
			xrand = rand() % xdim;
			yrand = rand() % ydim;
			//Checks if taken, but only if it is not the first time
			if (i > 0) {
				for (int j = i - 1; j >= 0; j--) {
					if ((xrand == minelocs[j][0]) && (yrand == minelocs[j][1])) {
						loctaken = true;
						break;
					}
					else {
						loctaken = false;
					}
				}
			}
			else {
				loctaken = false;
			}
		}
		minelocs[i].push_back(xrand);
		minelocs[i].push_back(yrand);
		gridsqs[xrand][yrand].hasmine = true;
		//Add mine counter to adjacent squares that exist
		//Minus 1 because of 0 index
		if ((xrand != xdim-1) && (yrand != ydim-1)) gridsqs[xrand + 1][yrand + 1].minenum++;
		if ((xrand != xdim-1)) gridsqs[xrand + 1][yrand].minenum++;
		if ((xrand != xdim-1) && (yrand != 0)) gridsqs[xrand + 1][yrand - 1].minenum++;
		if ((yrand != 0)) gridsqs[xrand][yrand - 1].minenum++;
		if ((xrand != 0) && (yrand != 0)) gridsqs[xrand - 1][yrand - 1].minenum++;
		if ((xrand != 0)) gridsqs[xrand - 1][yrand].minenum++;
		if ((xrand != 0) && (yrand != ydim-1)) gridsqs[xrand - 1][yrand + 1].minenum++;
		if ((yrand != ydim-1)) gridsqs[xrand][yrand + 1].minenum++;
	}
	int choice, xchoice, ychoice; //user choice
	bool gameover = false, moretoclear = true, userwon = false; //for if user has lost or quit
	while (!gameover) {

		//Displays grid
		cout << "   0  1  2  3  4  5  6  7  8  9" << endl;
		cout << "   ----------------------------" << endl;
		for (int i = 0; i < gridsqs.size(); i++) {
			cout << i << "| ";
			for (int j = 0; j < gridsqs[i].size(); j++) {
				gridsqs[i][j].displaysq();
			}
			cout << endl;
		}
		//Prompts user
		cout << "\nEnter 1 to clear area, 2 to add or remove flag, or any other key to quit. \n";
		cin >> choice;
		if ((choice != 1) && (choice != 2)) {
			gameover = true;
			break;
		}
		else if (choice == 1) {
			//Make sure a valid input is made
			xchoice = -1;
			ychoice = -1;
			while ((ychoice > (ydim - 1)) || (ychoice < 0)) {
				cout << "\nEnter x coordinate: ";
				cin >> ychoice; //Opposite because of matrix
				if (!cin) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					ychoice = -1;
				}
			}
			while ((xchoice > (xdim - 1)) || (xchoice < 0)) {
				cout << "\nEnter y coordinate: ";
				cin >> xchoice;
				if (!cin) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					xchoice = -1;
				}
			}
			//If successfully cleared, then update status
			if (gridsqs[xchoice][ychoice].clearing()) {
				gridsqs[xchoice][ychoice].cleared = true;
				//Clear more squares
				if (gridsqs[xchoice][ychoice].minenum == 0) {
					vector<vector<int>> checkthese; //Vector to which squares surrounding what
					checkthese.push_back({ xchoice, ychoice });
					vector<vector<int>> tempfor_checkthese;
					while (moretoclear) {
						for (int i = 0; i < checkthese.size(); i++) {
							//Look up
							if ((checkthese[i][0] != xdim - 1) && (gridsqs[checkthese[i][0] + 1][checkthese[i][1]].hasmine == false) && (gridsqs[checkthese[i][0] + 1][checkthese[i][1]].cleared == false)) {
								gridsqs[checkthese[i][0] + 1][checkthese[i][1]].cleared = true;
								if (gridsqs[checkthese[i][0] + 1][checkthese[i][1]].minenum == 0) {
									tempfor_checkthese.push_back({ (checkthese[i][0] + 1),(checkthese[i][1]) }); //Put in vector for next iteration
								}
							}
							//Look down
							if ((checkthese[i][0] != 0) && (gridsqs[checkthese[i][0] - 1][checkthese[i][1]].hasmine == false) && (gridsqs[checkthese[i][0] - 1][checkthese[i][1]].cleared == false)) {
								gridsqs[checkthese[i][0] - 1][checkthese[i][1]].cleared = true;
								if (gridsqs[checkthese[i][0] - 1][checkthese[i][1]].minenum == 0) {
									tempfor_checkthese.push_back({ (checkthese[i][0] - 1),(checkthese[i][1]) });
								}
							}
							//Look right
							if ((checkthese[i][1] != ydim - 1) && (gridsqs[checkthese[i][0]][checkthese[i][1] + 1].hasmine == false) && (gridsqs[checkthese[i][0]][checkthese[i][1] + 1].cleared == false)) {
								gridsqs[checkthese[i][0]][checkthese[i][1] + 1].cleared = true;
								if (gridsqs[checkthese[i][0]][checkthese[i][1] + 1].minenum == 0) {
									tempfor_checkthese.push_back({ (checkthese[i][0]),(checkthese[i][1] + 1) });
								}
							}
							//Look left
							if ((checkthese[i][1] != 0) && (gridsqs[checkthese[i][0]][checkthese[i][1] - 1].hasmine == false) && (gridsqs[checkthese[i][0]][checkthese[i][1] - 1].cleared == false)) {
								gridsqs[checkthese[i][0]][checkthese[i][1] - 1].cleared = true;
								if (gridsqs[checkthese[i][0]][checkthese[i][1] - 1].minenum == 0) {
									tempfor_checkthese.push_back({ (checkthese[i][0]),(checkthese[i][1] - 1) });
								}
							}
						}
						checkthese.clear();
						checkthese = tempfor_checkthese;
						tempfor_checkthese.clear();
						if (checkthese.size() == 0) {
							break;
						}
					}
				}
			}
			//If mine then end game
			else {
				cout << "MINE HIT\nGAME OVER";
				gameover = true;
				break;
			}
		}
		else if (choice == 2) {
			//Make sure a valid input is made
			xchoice = -1;
			ychoice = -1;
			while ((ychoice > (ydim - 1)) || (ychoice < 0)) {
				cout << "\nEnter x coordinate: ";
				cin >> ychoice; //Opposite because of matrix
				if (!cin) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					ychoice = -1;
				}
			}
			while ((xchoice > (xdim - 1)) || (xchoice < 0)) {
				cout << "\nEnter y coordinate: ";
				cin >> xchoice;
				if (!cin) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					xchoice = -1;
				}
			}
			if (gridsqs[xchoice][ychoice].hasflag == false) {
				gridsqs[xchoice][ychoice].hasflag = true;
			}
			else {
				gridsqs[xchoice][ychoice].hasflag = false;
			}
		}
		//Check win condition
		for (int i = 0; i < gridsqs.size(); i++) {
			for (int j = 0; j < gridsqs[i].size(); j++) {
				if (((gridsqs[i][j].hasmine == false) && (gridsqs[i][j].cleared == true)) || ((gridsqs[i][j].hasmine == true) && (gridsqs[i][j].cleared == false))) {
					gameover = true;
					userwon = true;
				}
				else {
					gameover = false;
					userwon = false;
					goto endofwincheck;
				}
			}
		}
	endofwincheck:
		continue;
	}
	if (userwon) cout << "YOU WON\nGAME OVER";
	return 0;
}