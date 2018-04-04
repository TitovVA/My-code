#include <iostream>
#include <set>
#include <iterator>
#include <cstdlib>

#include "Sfe.h"

using namespace std;

vector <vector <simple_sfe*> > S;

void check_yn(sfe* A,int n){//cheeck for passing y on its value set
	for (unsigned int i = 0; i < A->x_val.size(); ++i){
		*(A->x_val[i]) = 0;
	}
	for (unsigned int i = 0; i < A->x_val.size(); ++i){
		if (((n >> i ) & 1) == 1){
				*(A->x_val[i]) = 1;
		}
	}
	*(A->y_val[0]) = 0;
	if (A->calculate() != 0){
		return;
	}
	*(A->y_val[0]) = 1;
	if (A->calculate() != 1){
		return;
	}
	S[n].push_back(new sfe(A)); // if all ok add sfe
}

void put_x(sfe* A,unsigned int j,unsigned int i){//Brute x combination
	if (j == A->all_elem.back().size()){
		for (unsigned int i = 0; i < 8; i++){ ////////////////////////////////////////////////////////////////8
			A->y.clear();
			A->y.push_back(i);
			check_yn(A,i);
		}
		return;
	}
	if (j == i){
		put_x(A,j + 1,i);
		return;
	}
	if ((j % 2 == 0)||(j == i + 1)){
		for (unsigned int x = 0; x < A->x_val.size(); x++){
			A->set_var(j,'A' + x);
			put_x(A,j + 1,i);
		}
	}
	else{
		if (A->all_elem[A->deep-1][int(j/2)]->operation == 'n'){
			put_x(A,j + 1,i);
			return;
		}
		for (unsigned int x = 0; int(x + 'A') <= A->all_elem.back()[j-1]->operation; x++){
			A->set_var(j,x + 'A');
			put_x(A,j + 1,i);
		}
	}
	A->set_var(j);
}

void put_y(sfe* A){//Brute y position
	for (unsigned int i = 0; i < A->all_elem.back().size(); i++){
		A->set_var(i);
	}
	for (unsigned int i = 0; i < A->all_elem.back().size(); i = i + 2){
		if (A->all_elem[A->deep-1][int(i/2)]->operation != 'n'){
			A->y.clear();
			A->y.push_back(0);
			A->set_var(i,'0');
			put_x(A,0,i);
		}
	}
}

void brute(sfe* A, int i, int j,int deep, int fl){//Brute combination of func_elem
	if (i >= deep){
		put_y(A);
		return;
	}
	if (j >= (1<<i)){
		brute(A,i+1,0,deep,0);
		return;
	}
	brute(A,i,j+1,deep,!(fl));	
	if (fl == 0){
		A->all_elem[i][j]->operation = '&';
		brute(A,i,j+1,deep,0);
	}
	if ((i == deep - 1)&&(j%2 == 0)){
		A->all_elem[i][j]->operation = 'n';
		brute(A,i,j+1,deep,0);
	}
	A->all_elem[i][j]->operation = 'v';
}

int main(int argc, char** argv){
	try{
		int deep = 2;
		int x_n = 3;
		int y_n = 8;
		for (int i = 0; i < y_n; i++){
			S.push_back(vector <simple_sfe*>());
		}
		sfe* A = new sfe(deep,x_n,0);
		
		brute(A,0,0,deep,0);
		
		for (int i = 0; i < 8; ++i){
			cout <<"////////////////////////////////////////////////////////////////////////////////"<< i << ":\n";
			int q = 0;
			for (unsigned int j= 0; j < S[i].size(); ++j){
				S[i][j]->print();
				q++;
			}
			cout << "q: " << q << "\n";
		}
		
		/*vvs cc0,cc1;
		take_composite_candidate(S, cc0, cc1, y_n);
		*/
		vector <simple_sfe*> S_in;
		vector <simple_sfe*> S_out;
		for (unsigned int i = 0; i < S.size(); ++i){
			for (unsigned int j = 0; j < S[i].size(); ++ j){
				S_in.push_back(S[i][j]);
			}
		}
		cout << "y1: " << S_in.size() << "\n";
		
		vector <simple_sfe*> S_x;
		sfe* B = new sfe(deep,x_n);
		create_all_sfe_no_y(S_x,B,0,0,deep,0);
		cout << "x only: " << S_x.size() << "\n";
		
		vector <simple_sfe*> S_add;
		
		sfe* C = new sfe(deep,x_n,0,1);
		create_all_sfe_2_y(S_add,C,0,0,deep,0);
		cout << "y2: " << S_add.size() << "\n";
		
		composite(S_add,S_out);
		
		cout << "comp y2: " << S_out.size() << "\n";
		
		S_out.clear();
		
		for (unsigned int j = 0; j < S_add.size(); ++j){
			S_in.push_back(S_add[j]);
			S_x.push_back(S_add[j]);
		}
		cout << "y2 + y1: " << S_in.size() << "\n";
		
		composite(S_in,S_out);
		cout << "comp y2 +y1: " << S_out.size() << "\n";
		
		S_in = S_out;
		
		S_out.clear();
		
		S_add.clear();
		
		composite(S_x,S_add);
		cout << "comp y2 + x: " << S_add.size() << "\n";
		
		unsigned int i = 0;
		while (i < S_add.size()){
			if (!ch_y(S_add[i])){
				S_add.erase(S_add.begin()+i);
			}
			else if(S_add[i]->y.size() < 2){
					S_add.erase(S_add.begin()+i);
				}
				else {
				++i;
			}
		}
		cout << "sort y2 + x: " << S_add.size() << "\n";
		
		for (unsigned int j = 0; j < S_add.size(); ++j){
			S_in.push_back(S_add[j]);
		}
		cout << "comp + comp: " << S_in.size() << "\n";
		
		composite(S_in,S_out);
		
		S_in = S_out;
		c(S_in);
		
		for (unsigned int i = 0; i < S_in.size(); ++i){
			S_in[i]->print();
		}
		
		cout << "k: " << S_in.size() << "\n";
		
		
		
		S_out.clear();
		
		composite(S_in,S_out);
		
		
		cout << "k: " << S_out.size() << "\n";
		/*
		for (unsigned int i = 0; i < cc0.size(); i++){
			cout << "y: " << i << "\n";
			for (unsigned int j = 0; j < cc0[i].size(); j++){
				cout << "sfe #" << j << ": ";
				copy( cc0[i][j].begin(), cc0[i][j].end(), ostream_iterator<int>(cout," "));
				cout << "\n";
			}
			cout << "\n";
		}
		*/
		
	}
	catch(const char* c){
		cout <<c;
		return 1;
	}
	catch(int c){
		cout << "\n"<< char(c) <<"\n";
		return 1;
	}
}
