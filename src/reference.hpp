/*
 *
 * reference.hpp
 * Header file for reference.cpp
 *
 */
#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <unordered_map>
#include <string>
#include <tuple>

#include "link_function.hpp" // Include dlib and column_vector

class Reference
{
    public:
        Reference();
        Reference(const std::string& name, 
                  const std::vector<std::string>& filenames, 
                  const std::vector<size_t>& kmer_lengths,
                  const size_t sketchsize64, 
                  const uint8_t min_count); // read and run sketch

        Reference(const std::string& name,
                  const size_t bbits,
                  const size_t sketchsize64); // For loading from DB
        
        const std::vector<uint64_t> & get_sketch(const int kmer_len) const;
        void add_kmer_sketch(const std::vector<uint64_t>& sketch, const int kmer_len);
        void remove_kmer_sketch(const size_t kmer_len);
        double jaccard_dist(const Reference &query, const int kmer_len) const;
        std::tuple<float, float> core_acc_dist(const Reference &query) const;
        std::tuple<float, float> core_acc_dist(const Reference &query, const std::vector<size_t> &kmers) const;
        std::vector<size_t> kmer_lengths() const;

        std::string name() const { return _name; }
        size_t bbits() const { return _bbits; }
        size_t sketchsize64() const { return _sketchsize64; }

        // For sorting, order by name
        friend bool operator < (Reference const & a, Reference const & b)
        { return a._name < b._name; }
        friend bool operator == (Reference const & a, Reference const & b)
        { return a._name == b._name; }

    private:
        // Info
        std::string _name;
        size_t _bbits;
        size_t _sketchsize64;

        // sketch - map keys are k-mer length
        std::unordered_map<int, std::vector<uint64_t>> usigs;
};

// Defined in linear_regression.cpp
std::tuple<float, float> regress_kmers(const Reference * r1, 
                                       const Reference * r2, 
                                       const std::vector<size_t>& kmers);

// Need T -> double to be possible
template <class T>
column_vector vec_to_dlib(const std::vector<T>& invec)
{
    column_vector dlib_vec;
    dlib_vec.set_size(invec.size());
    for (unsigned int i = 0; i < invec.size(); i++)
    {
        dlib_vec(i) = invec.at(i);
    }
    return(dlib_vec);
}

// Defined in linear_regression.cpp
dlib::matrix<double,0,2> add_intercept(const column_vector& kmer_vec);