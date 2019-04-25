#ifndef INC_ARDUTILS_HPP
#define INC_ARDUTILS_HPP

#include "Python.hpp"
#include <string>

namespace py = pybind11;

std::vector<float> read_field_ir( std::vector<int> size, std::vector<float> listener,
            const std::string &prefix, std::size_t nsteps );

std::vector<float> read_field_irf( std::vector<int> size, std::vector<float> listener,
            const std::string &prefix, std::size_t nsteps );

std::vector< std::vector<float> > read_field_irs( std::vector<int> size, std::vector< std::vector<float> > listeners,
                                  const std::string &_prefix, std::size_t _nsteps );

#endif  // INC_ARDUTILS_HPP
