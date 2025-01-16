#include "simulator.h"
#include "config.hpp"
#include "stats.h"
#include "perceptron_freq_predictor.h"

PerceptronFreqPredictor::PerceptronFreqPredictor()
   : m_correct_predictions(0)
   , m_incorrect_predictions(0)
{
}

PerceptronFreqPredictor::PerceptronFreqPredictor(String name, core_id_t core_id)
   : m_correct_predictions(0)
   , m_incorrect_predictions(0)
{
  registerStatsMetric(name, core_id, "num-correct", &m_correct_predictions);
  registerStatsMetric(name, core_id, "num-incorrect", &m_incorrect_predictions);
}

PerceptronFreqPredictor::~PerceptronFreqPredictor()
{ }

int PerceptronFreqPredictor::m_history_size;
int PerceptronFreqPredictor::m_table_size;

PerceptronFreqPredictor* PerceptronFreqPredictor::create(core_id_t core_id)
{
  	PerceptronFreqPredictor* pred = new PerceptronFreqPredictor();
      config::Config *cfg = Sim()->getCfg();
      assert(cfg);

      m_history_size = cfg->getIntArray("perf_model/perceptron_freq_predictor/history_size", core_id);
      m_table_size = cfg->getIntArray("perf_model/perceptron_freq_predictor/perceptron_table_size", core_id);

      std::cout << "---------- CREATE PERCEPTRON ---------- Core = " << core_id << std::endl;
      std::cout << "History Size = " << m_history_size << std::endl;
      std::cout << "Table Size = " << m_table_size << std::endl;

      pred->perceptron_table = (PerceptronRow *)malloc(m_table_size * sizeof(PerceptronRow));

      for (int i = 0; i < m_table_size; i++) {
        pred->perceptron_table[i].weight = (int *)malloc(m_history_size * sizeof(int));

	    for (int j = 0; j < m_history_size; j++) {
          pred->perceptron_table[i].weight[j] = 1;
        }
      }

      pred->history_register.weight = (int *)malloc(m_history_size * sizeof(int));

      for (int i = 0; i < m_history_size; i++) {
        if (i % 2 == 0) {
          pred->history_register.weight[i] = 1;
        } else {
          pred->history_register.weight[i] = -1;
        }
      }

      std::cout << "======== PerceptronTable ========" << std::endl;
      for (int i = 0; i < m_table_size; i++) {
        for (int j = 0; j < m_history_size; j++) {
          std::cout << pred->perceptron_table[i].weight[j] << " ";
        }
        std::cout << std::endl;
      }

      std::cout << "======== HistoryRegister ========" << std::endl;
      for (int i = 0; i < m_history_size; i++) {
        std::cout << pred->history_register.weight[i] << " ";
      }
      std::cout << std::endl;

      return pred;
}

void PerceptronFreqPredictor::resetCounters()
{
  m_correct_predictions = 0;
  m_incorrect_predictions = 0;
}

void PerceptronFreqPredictor::updateCounters(bool predicted, bool actual)
{
   if (predicted == actual)
      ++m_correct_predictions;
   else
      ++m_incorrect_predictions;
}

bool PerceptronFreqPredictor::predict(IntPtr ip)
{
  int index = ip % PerceptronFreqPredictor::m_table_size;
  int sum = 0;

  for (int i = 0; i < m_history_size; i++) {
    sum += perceptron_table[index].weight[i] * history_register.weight[i];
  }

  if (sum >= 0){
    return true;
  }

  return false;
}

void PerceptronFreqPredictor::update(bool predicted, bool actual, IntPtr ip)
   {
      int index = ip % PerceptronFreqPredictor::m_table_size;

      if (actual)
      {
        for (int i = 0; i < PerceptronFreqPredictor::m_history_size; i++) {
          perceptron_table[index].weight[i] += 1;
        }

        if (predicted) {
           m_correct_predictions += 1;
        } else {
          m_incorrect_predictions += 1;
        }

      } else {
        for (int i = 0; i < PerceptronFreqPredictor::m_history_size; i++) {
          perceptron_table[index].weight[i] -= 1;
        }

        if (!predicted) {
           m_correct_predictions += 1;
        } else {
          m_incorrect_predictions += 1;
        }
      }

      for (int i = PerceptronFreqPredictor::m_history_size - 1 ; i >= 0; i--) {
        history_register.weight[i] = history_register.weight[i-1];
      }

      if (actual) {
        history_register.weight[0] = 1;
      } else {
        history_register.weight[0] = -1;
      }
   }

