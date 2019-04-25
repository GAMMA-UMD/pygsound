#ifndef INC_METRICS_HPP
#define INC_METRICS_HPP

#include "Python.hpp"
#include <vector>

namespace py = pybind11;

std::vector< double > a_weighting( const std::vector< double > &freqs );
double a_noise_level( std::vector<double> signal, double rate );
std::vector<double> a_noise_bands( std::vector<double> signal, double rate );

#endif  // INC_METRICS_HPP
