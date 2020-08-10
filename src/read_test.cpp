
#include <fstream>
#include <iostream>

#include "reference.hpp"
#include "database.hpp"
#include "random_match.hpp"
#include "api.hpp"
#include "gpu.hpp"
#include "countmin.hpp"
#include "seqio.hpp"

int main (int argc, char* argv[])
{
    // Runs a test of functionality
    
    std::vector<size_t> kmer_lengths {15, 29};
    SeqBuf ref_seq({argv[2], argv[3]}, kmer_lengths.back());
    Reference ref(argv[1], ref_seq, kmer_lengths, 156, true, 20, false);
    SeqBuf query_seq({argv[5], argv[6]}, kmer_lengths.back());
    Reference query(argv[4], query_seq, kmer_lengths, 156, true, 20, false);

    RandomMC random(true);
    std::cout << ref.jaccard_dist(query, 15, random) << std::endl;
    std::cout << ref.jaccard_dist(query, 29, random) << std::endl;

    auto core_acc = ref.core_acc_dist<RandomMC>(query, random); 
    std::cout << std::get<0>(core_acc) << "\t" << std::get<1>(core_acc) << std::endl;

    create_sketches_cuda("gpu_db",
                   {argv[1], argv[4]},
                   {{argv[2], argv[3]}, {argv[5], argv[6]}},
                   kmer_lengths,
                   156,
                   true,
                   20,
				   4,
                   0);

    return 0;
}

