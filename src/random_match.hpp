/*
 *
 * random_match.hpp
 * Header file for random_match.cpp
 *
 */
#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <tuple>

#include "robin_hood.h"
#include "matrix.hpp"

const unsigned int default_n_clusters = 3;
const unsigned int default_n_MC = 5;

class Reference;

class RandomMC {
	public:
		// no adjustment
		RandomMC(); 
		// no MC - use simple Bernoulli prob      
		RandomMC(const bool use_rc); 
		// set up MC from sketches
		RandomMC(const std::vector<Reference>& sketches, 
				   unsigned int n_clusters,
				   const unsigned int n_MC,
				   const bool use_rc,
				   const int num_threads); 
		// load MC from database (see database.cpp)
		RandomMC(const bool use_rc,
		         const unsigned int min_k,
				 const unsigned int max_k,
				 const robin_hood::unordered_node_map<std::string, uint16_t>& cluster_table,
				 const robin_hood::unordered_node_map<size_t, NumpyMatrix>& matches,
				 const NumpyMatrix& cluster_centroids)
			: _no_adjustment(false), _no_MC(false), _use_rc(use_rc), _min_k(min_k), _max_k(max_k),
              _cluster_table(cluster_table), _matches(matches), _cluster_centroids(cluster_centroids)
		{}

		double random_match(const Reference& r1, const Reference& r2, const size_t kmer_len) const;
        size_t closest_cluster(const Reference& ref) const;
		void add_query(const Reference& query);
		// TODO add flatten functions here too
		// will need a lookup table (array) from sample_idx -> random_match_idx

		// functions for saving
		robin_hood::unordered_node_map<std::string, uint16_t> cluster_table() const { return _cluster_table; }
		robin_hood::unordered_node_map<size_t, NumpyMatrix> matches() const { return _matches; }
		NumpyMatrix cluster_centroids() const { return _cluster_centroids; }
		std::tuple<unsigned int, unsigned int> k_range() const { return std::make_tuple(_min_k, _max_k); }
		bool use_rc() const { return _use_rc; }

	private:
		unsigned int _n_clusters;
		bool _no_adjustment;
        bool _no_MC;
		bool _use_rc;

		unsigned int _min_k;
		unsigned int _max_k;
		
		// name index -> cluster ID
		robin_hood::unordered_node_map<std::string, uint16_t> _cluster_table;
		std::vector<std::string> _representatives;
		// k-mer idx -> square matrix of matches, idx = cluster
		robin_hood::unordered_node_map<size_t, NumpyMatrix> _matches; 

        NumpyMatrix _cluster_centroids;
};