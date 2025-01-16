#ifndef PERCEPTRON_FREQ_PREDICTOR_H
#define PERCEPTRON_FREQ_PREDICTOR_H

#include <iostream>
#include "perceptron_row.h"

class PerceptronFreqPredictor
{
public:
   PerceptronFreqPredictor();
   PerceptronFreqPredictor(String name, core_id_t core_id);
   virtual ~PerceptronFreqPredictor();

   bool predict(IntPtr ip);
   void update(bool predicted, bool actual, IntPtr ip);

   UInt64 getMispredictPenalty();
   static PerceptronFreqPredictor* create(core_id_t core_id);

   UInt64 getNumCorrectPredictions() { return m_correct_predictions; }
   UInt64 getNumIncorrectPredictions() { return m_incorrect_predictions; }

   void resetCounters();

protected:
   void updateCounters(bool predicted, bool actual);

public:
   UInt64 m_correct_predictions;
   UInt64 m_incorrect_predictions;

   static int m_table_size;
   static int m_history_size;

   PerceptronRow *perceptron_table;
   PerceptronRow history_register;

   enum Freqencies {
     LOW_FREQUENCY = 2000,
     HIGH_FREQUENCY = 2660,
   };
};

#endif
