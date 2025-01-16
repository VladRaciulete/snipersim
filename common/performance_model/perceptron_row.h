#ifndef PERCEPTRON_ROW_H
#define PERCEPTRON_ROW_H

#include <iostream>

#include "fixed_types.h"

class PerceptronRow
{
public:
	int *weight;
	PerceptronRow();
	~PerceptronRow();
};

#endif
