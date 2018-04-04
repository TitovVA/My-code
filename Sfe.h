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

////////////////////////////////////////////////////////////

class simple_sfe{
public:
	vector <int> y;
	virtual bool calculate() = 0;
	virtual void set_y(int y_num, bool value) = 0;
	virtual void set_x(int x_num, bool value) = 0;
	virtual void print() const = 0;
};
/////////
class sfe_composition : public simple_sfe{
	
	simple_sfe * first,*second;
	
public:
	char combine_operation;
	
	sfe_composition(simple_sfe * f, simple_sfe * s, char op, int f_i = 0, int s_i = 0);
	
	bool calculate();
	void set_y(int y_num, bool value);
	void set_x(int x_num, bool value);
	void print() const;
};
////////////
class sfe : public simple_sfe{
	
	element* initial(int d);
	
public:
	int deep; 
	vector <bool*> x_val;
	vector <bool*> y_val;
	vector < vector <element*> > all_elem;
	
	sfe(int d, int x_num, int y_n = -1,int y2_n = -1);
	sfe(sfe * const S);
	void set_var(int input, int v = -1);
	
	bool calculate();
	void set_y(int y_num, bool value);
	void set_x(int x_num, bool value);
	void print() const;
};


/*
void take_composite_candidate(vector < vector < simple_sfe*> > & S, 
											vvs & composite_candidate_0,
											vvs & composite_candidate_1,
											int y_n);
*/

void composite( vector < simple_sfe*> & S_in, vector < simple_sfe*> & S_out);

void c(vector < simple_sfe*> & S);

void create_all_sfe_no_y(vector <simple_sfe*> & S, sfe* A, int i, int j,int deep, int fl);
void create_all_sfe_2_y(vector <simple_sfe*> & S, sfe* A, int i, int j,int deep, int fl);
bool ch_y(simple_sfe* S);
#endif
