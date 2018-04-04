#include "Sfe.h"

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
