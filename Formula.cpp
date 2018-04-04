#include "Formula.h"

#include <iostream>
#include <stdexcept>

variable::variable(bool* var_pointer,char op){
	var = var_pointer;
	operation = op;
}

bool variable::calculate(){
	return (*var);
}

func_elem::func_elem(char bool_operation, element* first_operand, element* second_operand){
	operation = bool_operation;
	op1 = first_operand;
	op2 = second_operand;
}

bool func_elem::calculate(){
	if (operation == 'n'){
		return ( ! (op1->calculate()) );
	}
	if (operation == 'v'){
		return ( (op1->calculate()) || (op2->calculate()));
	}
	if (operation == '&'){
		return ( (op1->calculate()) && (op2->calculate()));
	}
	throw("f_elem: unknown operation");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

element* sfe::initial(int d){
	if (d == deep){
		all_elem[d].push_back(new variable(vars[0],'0'));
	}
	else{
		all_elem[d].push_back( new func_elem( 'v', initial(d+1), initial(d+1) ) );
	}
	return (all_elem[d].back());
}

sfe::sfe(int deep_of_sfe,int var_num){
	if (deep_of_sfe < 0){
		throw ("sfe: constuctor: d < 0!");
	}
	if (var_num < 1){
		throw ("sfe: constuctor: v < 1!");
	}
	deep = deep_of_sfe;
	for (int i = 0; i < var_num; ++i){
		vars.push_back(new bool(0));
	}
	for (int i = 0; i < deep +1; ++i){
		all_elem.push_back(vector<element*>());
	}
	initial(0);
}

sfe::sfe(sfe * const S){
	deep = S->deep;
	for (unsigned int i = 0; i < S->vars.size(); ++i){
		vars.push_back(new bool(0));
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
		set_var(j,S->all_elem[S->all_elem.size()-1][j]->operation - '0');
	}
}

void sfe::print() const{
	for (unsigned int i = 0; i < all_elem.size(); ++i){
		for (unsigned int j = 0; j < all_elem[i].size(); ++j){
			cout << all_elem[i][j]->operation << " ";
		}
		cout << "\n";
	}
	cout << "\n";
	for (unsigned int i = 0; i < vars.size(); ++i){
		cout << i << ": "<< *(vars[i]) << "\n";
	}
}

void sfe::set_var(int input, int v){
	if (v == -1){
		all_elem.back()[input]->var = NULL;
		all_elem.back()[input]->operation = 'x';
		return;
	}
	all_elem.back()[input]->var = vars[v];
	all_elem.back()[input]->operation = '0' + v;
}

bool sfe::calculate(){
	return(all_elem[0][0]->calculate());
}

sfe::~sfe(){
	for (unsigned int i = 0; i < all_elem.size(); ++i){
		for (unsigned int j = 0; j < all_elem[i].size(); ++j){
			delete all_elem[i][j];
		}
	}
	for (unsigned int i = 0; i < vars.size(); ++i){
		delete vars[i];
	}
}

bool sfe_pair2::calculate(){
	if (combine_operation == '&'){
		return (first->calculate() && second->calculate());
	}
	if (combine_operation == 'v'){
		return (first->calculate() || second->calculate());
	}
	throw ("sfe_pair2: calculate error!");
}
