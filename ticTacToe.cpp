#include<bits/stdc++.h>
using namespace std;
//at 3 can see 1 future user move
int maxDepth = 4;
int choice = 0;
int goalStateIndicator = 100;

struct Node {
	int h;
	bool maxMin;
	bool turn;
	bool board[3][3];
	bool filled[3][3];
	vector<struct Node*> children;
	struct Node *parent;
};

void printBoard(bool filled[3][3], bool board[3][3]) {
	cout<<"\n";
	for(int i=0; i<3; i++) {
		if(i != 0)
			cout<<"-----------\n";
		for(int j=0; j<3; j++) {
			if(filled[i][j]) {
				if(board[i][j])
					cout<<" X ";
				else
					cout<<" O ";
			} else
				cout<<"   ";
			if(j != 2)
				cout<<"|";
		}
		cout<<"\n";
	}
}

int sgn(int x) {
    if(x > 0)
    	return 1;
    else if(x < 0)
    	return -1;
    return 0;
}

int getEval(bool filled[3][3], bool board[3][3]) {
	int eval = 0;
	bool goalState = false;
	int w_lr = 0;
	int n_ele_lr = 0;
	int w_rl = 0;
	int n_ele_rl = 0;
	for(int i = 0; !goalState && i < 3; i++) {
		int w_row = 0;
		int n_ele_row = 0;
		int w_col = 0;
		int n_ele_col = 0;
		for(int j = 0; j < 3; j++) {
			if(filled[i][j]) {
				++n_ele_row;
				if(board[i][j])
					++w_row;
				else
					--w_row;
			}
			if(filled[j][i]) {
				++n_ele_col;
				if(board[j][i])
					++w_col;
				else
					--w_col;
			}

		} //ROW AND COL
		switch(n_ele_row) {
			case 1: eval += w_row;
					break;
			case 2: if(w_row == 0)
						break;
					else
						eval = eval + (sgn(w_row)*5);
					break;
			case 3: if(w_row == 3 || w_row == -3) {
						eval = (goalStateIndicator*sgn(w_row));
						goalState = true;
					} else if(w_row == 2 || w_row == -2) {
						eval = eval + (10*sgn(w_row)*(-1));
					}
					break;
		}
		if(!goalState) {
			switch(n_ele_col) {
				case 1: eval += w_col;
						break;
				case 2: if(w_col == 0)
							break;
						else
							eval = eval + (sgn(w_col)*5);
						break;
				case 3: if(w_col == 3 || w_col == -3) {
							eval = (goalStateIndicator*sgn(w_col));
							goalState = true;
						} else if(w_col == 2 || w_col == -2) {
						eval = eval + (10*sgn(w_col)*(-1));
						}
						break;
			}
		}
		if(!goalState) {
			if(filled[i][i]) {
				++n_ele_lr;
				if(board[i][i])
					++w_lr;
				else
					--w_lr;
			}
			if(filled[i][2-i]) {
				++n_ele_rl;
				if(board[i][2-i])
					++w_rl;
				else
					--w_rl;
			}
		} //DIAGONALS
	}
	if(!goalState) {
		switch(n_ele_lr) {
			case 1: eval += w_lr;
					break;
			case 2: if(w_lr == 0)
						break;
					else
						eval = eval + (sgn(w_lr)*5);
					break;
			case 3: if(w_lr == 3 || w_lr == -3) {
						eval = (goalStateIndicator*sgn(w_lr));
						goalState = true;
					} else if(w_lr == 2 || w_lr == -2) {
						eval = eval + (10*sgn(w_lr)*(-1));
					}
					break;
		}
	}
	if(!goalState) {
		switch(n_ele_rl) {
			case 1: eval += w_rl;
					break;
			case 2: if(w_rl == 0)
						break;
					else
						eval = eval + (sgn(w_rl)*5);
					break;
			case 3: if(w_rl == 3 || w_rl == -3) {
						eval = (goalStateIndicator*sgn(w_rl));
						goalState = true;
					} else if(w_rl == 2 || w_rl == -2) {
						eval = eval + (10*sgn(w_rl)*(-1));
					}
					break;
		}
	}
	return eval;
}

bool gameOver(bool filled[3][3], bool board[3][3]) {
	int eval = getEval(filled, board);
	if(eval == goalStateIndicator || eval == (goalStateIndicator*(-1)))
		return true;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(!filled[i][j])
				return false;
		}
	}
	return true;
}

int buildTree(int depth, struct Node *p, int &nodes) {
	//printBoard(p);
	//system("pause");
	++depth;
	if((depth+1) > maxDepth) {
		p->h = getEval(p->filled, p->board);
		return 0;
	}
	//system("cls");
	//cout<<depth<<endl<<nodes;
	int min = INT_MAX;
	int max = INT_MIN;
	int index = 0;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			if(!p->filled[i][j]) {
				struct Node *n = new Node;
				for(int k = 0; k < 3; k++) {
					for(int l = 0; l < 3; l++) {
						n->filled[k][l] = p->filled[k][l];
						n->board[k][l] = p->board[k][l];
					}
				}
				n->parent = p;
				n->maxMin = !(p->maxMin);
				n->turn = !(p->turn);
				n->board[i][j] = n->turn;
				n->filled[i][j] = true;
				++nodes;
				//printBoard(n);
				//system("pause");
				(p->children).push_back(n);
				buildTree(depth, n, nodes);
				if(p->maxMin) {
					if((n->h) > max) {
						max = n->h;
						index = (p->children).size()-1;
					}
				} else {
					if((n->h) < min) {
						min = n->h;
						index = (p->children).size()-1;
					}
				}
			}
		}
	}
	if(p->maxMin) {
		p->h = max;
		choice = index;
	} else {
		p->h = min;
		choice = index;
	}
	return 0;
}

/*
int cpuTurn(struct Node *n) {
	if((n->children).size() == 0)
		return (n->h);
	int min = INT_MAX;
	int max = INT_MIN;
	int max_index = 0;
	int min_index = 0;
	for(int i = 0; i < (n->children).size(); i++) {
		int temp = cpuTurn((n->children)[i]);
		if(n->maxMin) {
			if(temp > max) {
				max = temp;
				max_index = i;
			}
		} else {
			if(temp < min) {
				min = temp;
				min_index = i;
			}
		}
	}
	if(n->maxMin) {
		n->h = max;
		choice = max_index;
		return max;
	} else {
		n->h = min;
		choice = min_index;
		return min;
	}
}
*/

int expandTree(struct Node *x, int lvls) {
	//cout<<"Expand";
	if(((x->children).size() == 0 && lvls <= 1) || gameOver(x->filled, x->board)) {
		//cout<<"Flag";
		x->h = getEval(x->filled, x->board);
		return 0;
	}
	int min = INT_MAX;
	int max = INT_MIN;
	int index = 0;
	if((x->children).size() == 0) {
		//cout<<"Zero children";
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				if(!x->filled[i][j]) {
					struct Node *n = new Node;
					for(int k = 0; k < 3; k++) {
						for(int l = 0; l < 3; l++) {
							n->filled[k][l] = x->filled[k][l];
							n->board[k][l] = x->board[k][l];
						}
					}
					n->parent = x;
					n->maxMin = !(x->maxMin);
					n->turn = !(x->turn);
					n->board[i][j] = n->turn;
					n->filled[i][j] = true;
					(x->children).push_back(n);
					if(lvls > 0) {
						--lvls;
						expandTree(n, lvls);
					}
					if(x->maxMin) {
						if(n->h > max) {
							max = n->h;
							index = (x->children).size()-1;
						}
					} else {
						if(n->h < min) {
							min = n->h;
							index = (x->children).size()-1;
						}
					}
				}
			}
		}
		if(x->maxMin) {
			//cout<<"Maxing! ";
			x->h = max;
			choice = index;
		} else {
			//cout<<"Minning! ";
			x->h = min;
			choice = index;
		}
		return 0;
	}
	//cout<<"IN LOOP\n";
	for(int i = 0; i < (x->children).size(); i++) {
		expandTree((x->children)[i], lvls);
		if(x->maxMin) {
			if(((x->children)[i])->h > max) {
				max = (x->children)[i]->h;
				index = i;
			}
		} else {
			if(((x->children)[i])->h < min) {
				min = ((x->children)[i])->h;
				index = i;
			}
		}
	}
	if(x->maxMin) {
		//cout<<"Maxing! ";
		x->h = max;
		choice = index;
	} else {
		//cout<<"Minning! ";
		x->h = min;
		choice = index;
	}
	return 0;
}



int main() {
	//cout<<"Enter Max Depth(Min 2): ";
	//cin>>maxDepth;
	struct Node *hd = new Node;
	for(int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			hd->filled[i][j] = false;
			hd->board[i][j] = false;
		}
	}
	printBoard(hd->filled, hd->board);
	int usr;
	cout<<"\nMake your move. Enter location(1-9): ";
	cin>>usr;
	--usr;
	hd->board[usr/3][usr%3] = true;
	hd->filled[usr/3][usr%3] = true;
	printBoard(hd->filled, hd->board);
	int nodes = 1;
	hd->maxMin = false;
	hd->turn = true;
	buildTree(0, hd, nodes);
	hd = (hd->children)[choice];
	cout<<endl<<"\nMove made by CPU:\n";//<<choice<<" "<<hd->maxMin<<"\n";
	printBoard(hd->filled, hd->board);
	//cout<<endl<<nodes;
	//cpuTurn(hd);
	bool notOver = true;
	while(!gameOver(hd->filled, hd->board) && notOver) {
		do {
			cout<<"\nMake your move. Enter location(1-9): ";
			cin>>usr;
			--usr;
		}while(hd->filled[usr/3][usr%3]);
		//cout<<"While loop done\n";
		bool temp_filled[3][3], temp_board[3][3];
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				temp_filled[i][j] = hd->filled[i][j];
				temp_board[i][j] = hd->board[i][j];
			}
		}
		temp_board[usr/3][usr%3] = true;
		temp_filled[usr/3][usr%3] = true;
		//cout<<"Temp initialised\n";
		if(!gameOver(temp_filled, temp_board)) {
			//cout<<"Game not over\n";
			bool matched = false;
			int k;
			for(k = 0; k < (hd->children).size(); k++) {
				bool notSame = false;
				for(int i = 0; !notSame && i < 3; i++) {
					for(int j = 0; !notSame && j < 3; j++) {
						if((((hd->children)[k])->filled[i][j] == temp_filled[i][j]) && (((hd->children)[k])->board[i][j] == temp_board[i][j]))
							continue;
						else
							notSame = true;
					}
				}
				if(!notSame)
					break;
			} //check which child is same as user move
			if((hd->children).size() == 0) {
				//cout<<"No children\n";
				hd->filled[usr/3][usr%3] = true;
				hd->board[usr/3][usr%3] = true;
				hd->maxMin = false;
				hd->turn = true;
				//cout<<"building tree\n";
				buildTree(0, hd, nodes);
				//cout<<"tree built\n";
			} else {
				hd = (hd->children)[k];
				//printBoard(hd->filled, hd->board);
				//system("pause");
				//buildTree(0, hd, nodes);
				//cout<<"\nBefore Expand\n";
				expandTree(hd, maxDepth-2);
				//cout<<"\nAfter Expand\n";
			}
			hd = (hd->children)[choice];
			cout<<endl<<"\nMove made by CPU:\n";//<<choice<<" "<<hd->maxMin<<"\n";
			printBoard(hd->filled, hd->board);
		} else {
			//cout<<"Game over segment\n";
			printBoard(temp_filled, temp_board);
			notOver = false;
		}
	}

	cout<<"\n\nGAME OVER!!!!!!\n\n";
	system("pause");
	return 0;
}