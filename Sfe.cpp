#include "Sfe.h"

#include <stdexcept>
#include <iterator>
#include <cstdlib>

sfe_composition::sfe_composition(simple_sfe * f, simple_sfe * s, char op, int f_i, int s_i){
	first = f;
	second = s;
	combine_operation = op;
	for (unsigned int i = 0; i < first->y.size(); i++){
		for (unsigned int j = 0; j < second->y.size(); j++){
			if ( first->y[i] == second->y[j]){
				throw("intersect of y\n");
			}
		}
		y.push_back(first->y[i]);
	}
	for (unsigned int j = 0; j < second->y.size(); j++){
		y.push_back(second->y[j]);
	}
}

bool sfe_composition::calculate(){
	if (combine_operation == '&'){
		return (first->calculate() && second->calculate());
	}
	if (combine_operation == 'v'){
		return (first->calculate() || second->calculate());
	}
	throw ("sfe_composition: calculate error!");
}

void sfe_composition::set_y(int y_num, bool value){
	for (unsigned int i = 0; i < first->y.size(); i++){
		if (first->y[i] == y_num){
			first->set_y(y_num,value);
			return;
		}
	}
	for (unsigned int i = 0; i < second->y.size(); i++){
		if (second->y[i] == y_num){
			second->set_y(y_num,value);
			return;
		}
	}
	throw("sfe_comb: set_y error!");
}

void sfe_composition::set_x(int x_num, bool value){
	first->set_x(x_num,value);
	second->set_x(x_num,value);
}

void sfe_composition::print() const{
	for (unsigned int i = 0; i < y.size(); i++){
		cout << y[i] << " ";
	}
	cout << "\n op:" << combine_operation << "\n";
	cout << "first:\n";
	//first->print();
	cout << "second:\n";
	//second->print();
}

/////////////////////////////////////////////////////////////////////////////////////

element* sfe::initial(int d){
	if (d == deep){
		all_elem[d].push_back(new variable(x_val[0],'A'));
	}
	else{
		all_elem[d].push_back( new func_elem( 'v', initial(d+1), initial(d+1) ) );
	}
	return (all_elem[d].back());
}

sfe::sfe(int d,int x_num,int y_n,int y2_n){
	deep = d;
	if (deep < 0){
		throw ("sfe: constuctor: d < 0!");
	}
	if (x_num < 1){
		throw ("sfe: constuctor: v < 1!");
	}
	if (y_n >= 0){
		y.push_back(y_n);
		y_val.push_back(new bool(0));
	}
	if (y2_n >= 0){
		y.push_back(y2_n);
		y_val.push_back(new bool(0));
	}
	for (int i = 0; i < x_num; ++i){
		x_val.push_back(new bool(0));
	}
	for (int i = 0; i < deep +1; ++i){
		all_elem.push_back(vector<element*>());
	}
	initial(0);
}

sfe::sfe(sfe * const S){
	deep = S->deep;
	y = S->y;
	for (unsigned int i = 0; i < S->x_val.size(); ++i){
		x_val.push_back(new bool(0));
	}
	for (unsigned int i = 0; i < S->y_val.size(); ++i){
		y_val.push_back(new bool(0));
	}
	for (int i = 0; i < deep +1; ++i){
		all_elem.push_back(vector<element*>());
	}
	initial(0);
	for (unsigned int i = 0; i < S->all_elem.size(); ++i){
		for (unsigned int j = 0; j < S->all_elem[i].size(); ++j){
			all_elem[i][j]->operation = S->all_elem[i][j]->operation;
		}
	}
	for (unsigned int j = 0; j < S->all_elem[S->all_elem.size()-1].size(); ++j){
		set_var(j,S->all_elem[S->all_elem.size()-1][j]->operation);
	}
}

void sfe::set_var(int input, int v){
	if (v == -1){
		all_elem.back()[input]->var = NULL;
		all_elem.back()[input]->operation = 'x';
		return;
	}
	all_elem.back()[input]->operation = v;
	if ((v >= '0')&&(v <='9')){
		all_elem.back()[input]->var = y_val[v-'0'];
		return;
	}
	if ((v >= 'A')&&(v <='Z')){
		all_elem.back()[input]->var = x_val[v-'A'];
		return;
	}
	if (v == 'x'){
		all_elem.back()[input]->var = NULL;
		return;
	}
	throw(v);
}

bool sfe::calculate(){
	return(all_elem[0][0]->calculate());
}

void sfe::set_y(int y_num, bool value){
	for (unsigned int i =0 ; i < y.size(); i++){
		if (y[i] == y_num){
			*(y_val[i]) = value;
			return;
		}
	}
	throw("set_y ERRR\n");
}

void sfe::set_x(int x_num, bool value){
	*(x_val[x_num]) = value;
}

void sfe::print() const{
	for (unsigned int i = 0; i < all_elem.size(); ++i){
		for (unsigned int j = 0; j < all_elem[i].size(); ++j){
			cout << all_elem[i][j]->operation << " ";
		}
		cout << "\n";
	}
	for (unsigned int i = 0; i < x_val.size(); ++i){
		cout << "           x"<< i << ": "<< *(x_val[i]) << "\n";
	}
	for (unsigned int i = 0; i < y_val.size(); ++i){
		cout << "           y"<< y[i] << ": "<< *(y_val[i]) << "\n";
	}
	cout << "                                y: ";
	for (unsigned int i = 0; i < y.size(); ++i){
		cout << y[i] << " ";
	}
	cout << "\n";
}
