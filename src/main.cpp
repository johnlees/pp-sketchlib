
#include <fstream>
#include <iostream>

#include "reference.hpp"
#include "database.hpp"
#include "api.hpp"

int main (int argc, char* argv[])
{
    // Runs a test of functionality
    
    std::vector<size_t> kmer_lengths {15, 17, 19, 21, 23, 25, 27, 29};
    Reference ref(argv[1], argv[2], kmer_lengths, 156);
    // Reference ref_copy(argv[1], argv[2], kmer_lengths);
    Reference query(argv[3], argv[4], kmer_lengths, 156);

    std::cout << ref.jaccard_dist(ref, 15) << std::endl;      // Should be 1
    std::cout << ref.jaccard_dist(query, 15) << std::endl;
    std::cout << ref.jaccard_dist(query, 29) << std::endl;
    std::cout << query.jaccard_dist(ref, 29) << std::endl;

    auto core_acc = ref.core_acc_dist(query); 
    std::cout << std::get<0>(core_acc) << "\t" << std::get<1>(core_acc) << std::endl;

    Database sketch_db("sketch.h5");
    sketch_db.add_sketch(ref);
    sketch_db.add_sketch(query);

    Reference ref_read = sketch_db.load_sketch(argv[1]);
    Reference query_read = sketch_db.load_sketch(argv[3]);
    std::cout << ref_read.jaccard_dist(query_read, 15) << std::endl;

    std::vector<Reference> ref_sketches = create_sketches("full",
                               {argv[1], argv[3]}, 
                               {argv[2], argv[4]}, 
                               kmer_lengths,
                               32,
                               2);

    MatrixXf dists = query_db(ref_sketches,
                              ref_sketches,
                              kmer_lengths,
                              2);

    std::cout << dists << std::endl;

    HighFive::File h5_db("listeria.h5");
    Database listeria_db(h5_db);
    std::ifstream rfile(argv[5]);
    std::string name, file;
    std::vector<Reference> listeria_sketches;
    while (rfile >> name >> file)
    {
        listeria_sketches.push_back(listeria_db.load_sketch(name));
    }
    MatrixXf listeria_dists = query_db(listeria_sketches,
                            listeria_sketches,
                            kmer_lengths,
                            4); 
    std::cout << listeria_dists << std::endl;

    return 0;
}
