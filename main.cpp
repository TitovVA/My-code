#include <iostream>

#include "Formula.h"
#include <set>
#include <iterator>
#include <cstdlib>

using namespace std;

typedef vector < vector < set <int> > >  vvs;

int k = 0;
int** y = 0;
vector < sfe*>* S = 0;
vvs composite_candidate_0, composite_candidate_1;

void initial_vvs(vvs & v,int size){
	for (int i = 0; i < size; i++){
		v.push_back(vector < set <int> > ());
	}
}

struct sfe_pair{
	sfe* first,*second;
	int f_id,s_id;
	sfe_pair(sfe* f, sfe* s,int i, int j){
		first = f;
		second = s;
		f_id = i;
		s_id = j;
	}
};


bool check_for_composite(sfe* A, int n, int b){
	for (unsigned int i = 0; i < A->vars.size(); ++i){
		*(A->vars[i]) = 0;
	}
	for (unsigned int i = 1; i < A->vars.size(); ++i){
		if (((n >> (i - 1) ) & 1) == 1){
				*(A->vars[i]) = 1;
		}
	}
	if (A->calculate() != b){
		return false;
	}
	*(A->vars[0]) = 1;
	if (A->calculate() != b){
		return false;
	}
	return true;
}

void check_yn(sfe* A,int n){//cheeck for passing y on its value set
	for (unsigned int i = 0; i < A->vars.size(); ++i){
		*(A->vars[i]) = 0;
	}
	for (unsigned int i = 1; i < A->vars.size(); ++i){
		if (((n >> (i - 1) ) & 1) == 1){
				*(A->vars[i]) = 1;
		}
	}
	if (A->calculate() != 0){
		return;
	}
	*(A->vars[0]) = 1;
	if (A->calculate() != 1){
		return;
	}
	S[n].push_back(new sfe(A)); // if all ok add sfe
}


void put_x(sfe* A,unsigned int j,unsigned int i){//Brute x combination
	if (j == A->all_elem.back().size()){
		for (unsigned int i = 0; i < 8; i++){
			check_yn(A,i);
		}
		return;
	}
	if (j == i){
		put_x(A,j + 1,i);
		return;
	}
	if ((j % 2 == 0)||(j == i + 1)){
		for (unsigned int x = 1; x < A->vars.size(); x++){
			A->set_var(j,x);
			put_x(A,j + 1,i);
		}
	}
	else{
		if (A->all_elem[A->deep-1][int(j/2)]->operation == 'n'){
			put_x(A,j + 1,i);
			return;
		}
		for (unsigned int x = 1; int(x + '0') <= A->all_elem.back()[j-1]->operation; x++){
			A->set_var(j,x);
			put_x(A,j + 1,i);
		}
	}
	A->set_var(j);
}

void check_y(sfe* A){//Brute y position
	for (unsigned int i = 0; i < A->all_elem.back().size(); i++){
		A->set_var(i);
	}
	for (unsigned int i = 0; i < A->all_elem.back().size(); i = i + 2){
		if (A->all_elem[A->deep-1][int(i/2)]->operation != 'n'){
			A->set_var(i,0);
			put_x(A,0,i);
		}
	}
}


void brute(sfe* A, int i, int j,int deep, int fl){//Brute combination of func_elem
	if (i >= deep){
		//A->print();
		//k++;
		//check_yo(A);
		check_y(A);
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

void combination_brute(int y_n){
	for (int n = 0; n < y_n; ++n){//for each yn
		for (unsigned int i = 0; i < S[n].size(); i++){//for each sfe of y[n]
			composite_candidate_0[n].push_back(  set <int> ());
			composite_candidate_1[n].push_back(  set <int> ());
			for (int y2 = 0; y2 < y_n; y2++){//for each another y2
				if (check_for_composite(S[n][i],y2,0)){ //if sfe can be combined with y2 on '0'
					composite_candidate_0[n].back().insert(y2);//add y2 to sfe combine set on '0'
					y[n][y2]++;
					k++;
				}
				if (check_for_composite(S[n][i],y2,1)){//also for '1'
					composite_candidate_1[n].back().insert(y2);
					y[n][y2]++;
					k++;
				}
			}
		}
	}
}

bool check_for_composite2(sfe_pair & A, int n, int b){
	for (unsigned int j = 0; j < A.first->vars.size(); ++j){
		*(A.first->vars[j]) = 0;
		*(A.second->vars[j]) = 0;
	}
	for (unsigned int j = 1; j <	A.first->vars.size(); ++j){
		if (((n >> (j - 1) ) & 1) == 1){
			*(A.first->vars[j]) = 1;
			*(A.second->vars[j]) = 1;
		}
	}
	if (((A.first->calculate()) || (A.second->calculate())) != b){
		return false;
	}
	*(A.first->vars[0]) = 0;
	*(A.second->vars[0]) = 1;
	if (((A.first->calculate()) || (A.second->calculate())) != b){
		return false;
	}
	*(A.first->vars[0]) = 1;
	*(A.second->vars[0]) = 0;
	if (((A.first->calculate()) || (A.second->calculate())) != b){
		return false;
	}
	*(A.first->vars[0]) = 1;
	*(A.second->vars[0]) = 1;
	if (((A.first->calculate()) || (A.second->calculate())) != b){
		return false;
	}
	return true;
}

void cheek_lvl2(vector <sfe_pair> & A){
	
}

int main(int argc, char** argv){
	try{
		/*
		element* A, *B;
		bool* f = new bool(0);
		bool* t = new bool(1);
		A = new variable(f);
		B = new variable(t);
		func_elem C('n',A);
		cout << C.calculate() << "\n";
		*/
		//sfe A(5,3);
		//A.print();
		int deep = 2;
		int v_n = 4;
		int y_n = 8;
		y = new int* [y_n];
		for (int i = 0; i < y_n; i++){
			y[i] = new int[y_n];
			for (int j = 0; j < y_n; j++){
				y[i][j] = 0;
			}
		}
		S = new vector <sfe*> [y_n];
		initial_vvs(composite_candidate_0,y_n);
		initial_vvs(composite_candidate_1,y_n);
		sfe* A = new sfe(deep,v_n);
		
		brute(A,0,0,deep,0); //after that S[i] contains all good sfe for yi
		
		/*for (int i = 0; i < 8; ++i){
			cout <<"////////////////////////////////////////////////////////////////////////////////"<< i << ":\n";
			int q = 0;
			for (unsigned int j= 0; j < S[i].size(); ++j){
				S[i][j]->print();
				q++;
			}
			cout << "q: " << q << "\n";
		}*/
		
		combination_brute(y_n);//after that composite_candidate_0 (1) contains set of posible yj to combine for each sfe of  each y
												//y - matrix of number of posible sfe of yi for yj
		
		cout  << "k:"<< k << "\n";// num of posible combinations
		
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				cout << y[i][j] << " ";
			}
			cout <<"\n";
		}
		for (unsigned int i = 0; i < composite_candidate_0.size(); i++){
			cout << "y: " << i << "\n";
			for (unsigned int j = 0; j < composite_candidate_0[i].size(); j++){
				cout << "sfe #" << j << ": ";
				copy( composite_candidate_0[i][j].begin(), composite_candidate_0[i][j].end(), ostream_iterator<int>(cout," "));
				cout << "\n";
			}
			cout << "\n";
		}
		for (unsigned int i = 0; i < composite_candidate_1.size(); i++){
			cout << "y: " << i << "\n";
			for (unsigned int j = 0; j < composite_candidate_1[i].size(); j++){
				cout << "sfe #" << j << ": ";
				copy( composite_candidate_1[i][j].begin(), composite_candidate_1[i][j].end(), ostream_iterator<int>(cout," "));
				cout << "\n";
			}
			cout << "\n";
		}
		
		vector < vector < sfe_pair > > y_pos0;
		vector < vector < sfe_pair > > y_pos1;
		
		for (int yi = 0; yi < y_n; yi++){//for each y
			y_pos0.push_back(vector <sfe_pair>());
			y_pos1.push_back(vector <sfe_pair>());
			for (unsigned int i = 0; i < S[yi].size(); i++){//for each yi sfe
				for (int y2 = 0; y2 < y_n; y2++){//for each posible y2
					for (unsigned int j = 0; j < S[y2].size(); j++){//for each posible sfe of y2
						if(composite_candidate_0[yi][i].find(y2) != composite_candidate_0[yi][i].end()){// if sfe_ i of yi can combine with y2
							if(composite_candidate_0[y2][j].find(yi) != composite_candidate_0[y2][j].end()){// sfe_ j of y2 can combine with y0
								y_pos0.back().push_back(sfe_pair(S[yi][i],S[y2][j],i + yi*1000,j+y2*1000));
							}
						}
						if(composite_candidate_1[yi][i].find(y2) != composite_candidate_1[yi][i].end()){// if sfe_ i of yi can combine with y2
							if(composite_candidate_1[y2][j].find(yi) != composite_candidate_1[y2][j].end()){// sfe_ j of y2 can combine with y0
								y_pos1.back().push_back(sfe_pair(S[yi][i],S[y2][j],i + yi*1000,j+y2*1000));
							}
						}
					}
				}
			}
		}
		/*
		vector < sfe_pair > y0_pos0;
		for (unsigned int i = 0; i < S[0].size(); i++){//for each yo sfe
			for (int y2 = 0; y2 < y_n; y2++){//for each posible y2
				for (unsigned int j = 0; j < S[y2].size(); j++){//for each posible sfe of y2
					if(composite_candidate_0[0][i].find(y2) != composite_candidate_0[0][i].end()){// if sfe_ i of yo can combine with y2
						if(composite_candidate_0[y2][j].find(0) != composite_candidate_0[y2][j].end()){// sfe_ j of y2 can combine with y0
							y0_pos0.push_back(sfe_pair(S[0][i],S[y2][j],i,j+y2*1000));
						}
					}
				}
			}
		}
		*/
		for (unsigned int j = 0; j < y_pos0.size(); j++){
			cout << j <<":\n";
			int q0 = 0;
			for (unsigned int i = 0; i < y_pos0[j].size(); i++){
				//cout << y_pos0[j][i].f_id << " " << y_pos0[j][i].s_id << "\n";
				q0++;
			}
			cout << q0 <<"\n";
			int q1 = 0;
			for (unsigned int i = 0; i < y_pos1[j].size(); i++){
				//cout << y_pos0[j][i].f_id << " " << y_pos0[j][i].s_id << "\n";
				q1++;
			}
			cout << q1 <<"\n";
		}
		
		/*
		
		int n1 = 7;
		int n2 = 4;
		unsigned int i = 0;
		while (i < y0_pos0.size()){//for each pair
			if (!check_for_composite2(y0_pos0[i],n1,0)){
				y0_pos0.erase(y0_pos0.begin() + i);
			}
			else if	(!check_for_composite2(y0_pos0[i],n2,0)){
				y0_pos0.erase(y0_pos0.begin() + i);
			}
			else{
				i++;
			}
		}
		int q = 0;
		for (unsigned int i = 0; i < y0_pos0.size(); i++){
			cout << y0_pos0[i].f_id << " " << y0_pos0[i].s_id << "\n";
			q++;
		}
		cout << q <<"\n";*/
		//S[3][3]->print();
	}
	catch(const char* c){
		cout <<c;
		return 1;
	}
}
