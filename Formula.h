#ifndef __Formula__
#define __Formula__

#include <iostream>
#include <vector>

using namespace std;

struct element{
	char operation;
	bool* var; //danger bidlokod var only in variable
	virtual bool calculate() = 0;
	virtual ~element(){};
};

struct variable : element{
	variable(bool* var_pointer,char op);
	bool calculate();
};

struct func_elem : element{
	element *op1, *op2;
	func_elem(char bool_operation, element* first_operand, element* second_operand = NULL);
	bool calculate();
};

////////////////////////////////////////////////////////////////////////////////////////////

class simple_sfe{
public:
	vector <bool*> vars;
	virtual bool calculate() = 0;
};

class sfe :public simple_sfe{
	element* initial(int d);
public:
	int deep;
	//vector <bool*> vars;
	vector < vector <element*> > all_elem;
	sfe(int deep_of_formula,int var_num);
	sfe(sfe * const S);
	void print() const;
	void set_var(int input, int v = -1);
	bool calculate();
	~sfe();
};

struct sfe_pair2 :public simple_sfe{
	simple_sfe * first,*second;
	int f_id,s_id;
	char combine_operation;
	sfe_pair2(simple_sfe* f, simple_sfe* s,int i, int j){
		first = f;
		second = s;
		f_id = i;
		s_id = j;
	}
	bool calculate();
};

#endif
