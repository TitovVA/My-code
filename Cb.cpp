#include "Sfe.h"

#include <vector>
#include <set> 

//typedef vector < vector < set <int> > >  vvs;

bool check_for_composite(simple_sfe* A, int n, bool b){
	for (unsigned int i = 0; i < 3; ++i){
		A->set_x(i, (n >> i) & 1 );
	}
	for (int k = 0; k < (1 << (A->y.size())); k++){
		for(unsigned int i = 0; i < A->y.size(); i++) {
			A->set_y(A->y[i],(k >> i) & 1);
		}
		if (A->calculate() != b){
			return false;
		}
	}
	return true;
}
/*
void take_composite_candidate(vector < vector < simple_sfe*> > & S, 
																		vvs & composite_candidate_0,
																		vvs & composite_candidate_1,
																		int y_n)
{
	int** control_matrix = new int* [S.size()];
	for (unsigned int i = 0; i < S.size(); i++){
		control_matrix[i] = new int[y_n];
		for (int j = 0; j < y_n; j++){
			control_matrix[i][j] = 0;
		}
	}
	
	
	for (unsigned int yn = 0; yn < S.size(); ++yn){//for each yn combination
		composite_candidate_0.push_back(vector < set <int> > ());
		composite_candidate_1.push_back(vector < set <int> > ());
		for (unsigned int i = 0; i < S[yn].size(); i++){//for each sfe_i of yn combination 
			composite_candidate_0[yn].push_back(  set <int> ());
			composite_candidate_1[yn].push_back(  set <int> ());
			for (int y = 0; y < y_n; y++){//for each another y
				if (check_for_composite(S[yn][i],y,0)){ //if sfe can be combined with y on '0'
					composite_candidate_0[yn].back().insert(y);//add y to sfe combine set on '0'
					control_matrix[yn][y]++;
				}
				if (check_for_composite(S[yn][i],y,1)){//also for '1'
					composite_candidate_1[yn].back().insert(y);
					control_matrix[yn][y]++;
				}
			}
		}
	}
	
	for (unsigned int i = 0; i < S.size(); i++){
		for (int j = 0; j < y_n; j++){
			cout << control_matrix[i][j] << " ";
		}
		cout << "\n";
	}
}
*/

bool can_comp(simple_sfe* A, simple_sfe* B, bool b){
	for (unsigned int i = 0; i < A->y.size(); ++i){
		for (unsigned int j = 0; j < B->y.size(); ++j){
			if (A->y[i] == B->y[j]){
					return false;
			}
		}
	}
	for(unsigned int i = 0; i < B->y.size(); i++) {
		if (!check_for_composite(A,B->y[i],b)){
			return false;
		}
	}
	return true;
}

void composite( vector < simple_sfe*> & S_in, vector < simple_sfe*> & S_out){
	for (unsigned int s1 = 0; s1 < S_in.size(); ++s1){//for each S1 sfe
		if (s1 % 100 == 0){
			cout << "s1: " << s1 << " / ";
		}
		for (unsigned int s2 = s1+1; s2 < S_in.size(); ++s2){//for each S2 sfe
			if (can_comp(S_in[s1],S_in[s2],0) && can_comp(S_in[s2],S_in[s1],0) ){
				S_out.push_back(new sfe_composition(S_in[s1],S_in[s2],'v'));
				/*if (S_out.back()->y.size() != S_in[0]->y.size()*2){
					S_out.back()->print();
					throw("err");
				}*/
				if (S_in.size() == 84){
					S_out.back()->print();
					throw("ura!\n");
				}
			}
			if (can_comp(S_in[s1],S_in[s2],1) && can_comp(S_in[s2],S_in[s1],1) ){
				S_out.push_back(new sfe_composition(S_in[s1],S_in[s2],'&'));
				/*if (S_out.back()->y.size() != S_in[0]->y.size()*2){
					S_out.back()->print();
					throw("err");
				}*/
				if (S_in.size() == 84){
					S_out.back()->print();
					throw("ura!\n");
				}
			}
		}
		if (s1 % 100 == 0){
			cout << " " << S_out.size() << "\n";
		}
	}
}

bool fin(simple_sfe* A,bool b){
	for (int i = 0; i  < 8; ++i){
		bool fl = false;
		for (unsigned int j = 0; j < A->y.size(); ++j){
			if (i == A->y[j]){
				fl = true;
				break;
			}
		}
		if (!fl){
			if (!check_for_composite(A, i, b)){
				return false;
			}
		}
	}
	return true;
}

void c(vector < simple_sfe*> & S){
	unsigned int i = 0;
	int u = 0;
	while ( i < S.size()){
		if (u % 1000 == 0){
			cout << u << "\n";
		}
		if (!fin(S[i],0)){
			if (!fin(S[i],1)){
				S.erase(S.begin()+i);
				u++;
			}
			else{
				cout << "1 ";
				++i;
			}
		}
		else{
			cout << "0 ";
			++i;
		}
		//if (i % 100 == 0){
			//cout << i << " / " << S.size() << "\n";
		//}
	}
}

void p_x(vector <simple_sfe*> & S, sfe* A,unsigned int j){//Brute x combination
	if (j == A->all_elem.back().size()){
		S.push_back(new sfe(A));
		return;
	}
	if (j % 2 == 0){
		for (unsigned int x = 0; x < A->x_val.size(); x++){
			A->set_var(j,'A' + x);
			p_x(S,A,j + 1);
		}
	}
	else{
		if (A->all_elem[A->deep-1][int(j/2)]->operation == 'n'){
			p_x(S,A,j + 1);
			return;
		}
		for (unsigned int x = 0; int(x + 'A') <= A->all_elem.back()[j-1]->operation; x++){
			A->set_var(j,x + 'A');
			p_x(S,A,j + 1);
		}
	}
	A->set_var(j);
}


void create_all_sfe_no_y(vector <simple_sfe*> & S, sfe* A, int i, int j,int deep, int fl){
	if (i >= deep){
		p_x(S,A,0);
		return;
	}
	if (j >= (1<<i)){
		create_all_sfe_no_y(S,A,i+1,0,deep,0);
		return;
	}
	create_all_sfe_no_y(S,A,i,j+1,deep,!(fl));
	if (fl == 0){
		A->all_elem[i][j]->operation = '&';
		create_all_sfe_no_y(S,A,i,j+1,deep,0);
	}
	if ((i == deep - 1)&&(j%2 == 0)){
		A->all_elem[i][j]->operation = 'n';
		create_all_sfe_no_y(S,A,i,j+1,deep,0);
	}
	A->all_elem[i][j]->operation = 'v';
}




bool ch_y(simple_sfe* S){
	for (unsigned int yi = 0; yi < S->y.size(); ++yi){
		S->set_y(S->y[yi],0);
	}
	for (unsigned int yi = 0; yi < S->y.size(); ++yi){
		for (int xi = 0; xi < 3; xi++){
			S->set_x(xi,(S->y[yi] >> xi) & 1);
		}
		for (int i = 0; i < 1 << S->y.size(); ++i){
			for (unsigned int j = 0; j < S->y.size(); ++j){
				S->set_y(S->y[j],(i >> j) & 1);
			}
			if (S->calculate() != ((i >> yi) & 1)){
				return false;
			}
		}
	}
	return true;
}


void p_y(vector <simple_sfe*> & S, sfe* A){//Brute y position
	for (unsigned int i = 0; i < A->all_elem.back().size(); i++){
		A->set_var(i);
	}
	A->set_var(0,'0');
	A->set_var(2,'1');
	for (unsigned int i = 0; i < 8; i++){
		A->y[0] = i;
		for (unsigned int j = 0; j < 8; j++){
			if (i != j){
				A->y[1] = j;
				for(int x1 = 0; x1 < 3; x1++){
					A->set_var(1,'A'+ x1);
					for(int x2 = 0; x2 < 3; x2++){
						A->set_var(3,'A'+ x2);
						if (ch_y(A)){
							S.push_back(new sfe(A));
						}
					}
				}
			}
		}
	}
}

void create_all_sfe_2_y(vector <simple_sfe*> & S, sfe* A, int i, int j,int deep, int fl){
	if (i >= deep){
		p_y(S,A);
		return;
	}
	if (j >= (1<<i)){
		create_all_sfe_2_y(S,A,i+1,0,deep,0);
		return;
	}
	create_all_sfe_2_y(S,A,i,j+1,deep,!(fl));
	if (fl == 0){
		A->all_elem[i][j]->operation = '&';
		create_all_sfe_2_y(S,A,i,j+1,deep,0);
	}
	A->all_elem[i][j]->operation = 'v';
}
