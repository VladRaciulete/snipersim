#include "simulator.h"
#include "branch_predictor.h"
#include "one_bit_branch_predictor.h"
#include "pentium_m_branch_predictor.h"
#include "a53branchpredictor.h"
#include "nn_branch_predictor.h"
#include "config.hpp"
#include "stats.h"
#include "perceptron_row.h"

PerceptronRow::PerceptronRow() { }

PerceptronRow::~PerceptronRow() { }
