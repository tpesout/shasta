#ifndef SHASTA_MIN_HASH_HPP
#define SHASTA_MIN_HASH_HPP

#include "Marker.hpp"
#include "MemoryMappedVectorOfVectors.hpp"
#include "MultitreadedObject.hpp"
#include "OrientedReadPair.hpp"
#include "ReadId.hpp"

namespace shasta {
    class MinHash;
}



// This class uses the MinHash algorithm to find candidate pairs
// of aligned reads. It uses as features
// sequences of m consecutive markers.
class shasta::MinHash :
    public MultithreadedObject<MinHash>{
public:

    // The constructor does all the work.
    MinHash(
        size_t m,                       // Number of consecutive markers that define a feature.
        size_t minHashIterationCount,   // Number of minHash iterations.
        size_t log2MinHashBucketCount,  // Base 2 log of number of buckets for minHash.
        size_t maxBucketSize,           // The maximum size for a bucket to be used.
        size_t minFrequency,            // Minimum number of minHash hits for a pair to be considered a candidate.
        size_t threadCount,
        const MemoryMapped::Vector<KmerInfo>& kmerTable,
        const MemoryMapped::VectorOfVectors<CompressedMarker, uint64_t>&,
        MemoryMapped::Vector<OrientedReadPair>&,
        const string& largeDataFileNamePrefix,
        size_t largeDataPageSize
);

private:

    // Store some of the arguments passed to the constructor.
    size_t m;                       // Number of consecutive markers that define a feature.
    size_t maxBucketSize;           // The maximum size for a bucket to be used.
    size_t minFrequency;            // Minimum number of minHash hits for a pair to be considered a candidate.
    size_t threadCount;
    const MemoryMapped::Vector<KmerInfo>& kmerTable;
    const MemoryMapped::VectorOfVectors<CompressedMarker, uint64_t>& markers;
    const string& largeDataFileNamePrefix;
    size_t largeDataPageSize;

    // Vectors containing only the k-mer ids of all markers
    // for all oriented reads.
    // Indexed by OrientedReadId.getValue().
    // This is used to speed up the computation of hash functions.
    MemoryMapped::VectorOfVectors<KmerId, uint64_t> kmerIds;
    void createKmerIds();
    void createKmerIds(size_t threadId);

    // The current MinHash iteration.
    // This is used to compute a different MurmurHash function
    // at each iteration.
    size_t iteration;

    // The min hash of each oriented read, at the current MinHash iteration.
    // Indexed by OrientedReadId::getValue().
    vector<uint64_t> minHash;
    void computeMinHash(size_t threadId);

    // The mask used to compute to compute the bucket
    // corresponding to a min hash value.
    uint64_t mask;

    // The buckets containing oriented read ids.
    MemoryMapped::VectorOfVectors<OrientedReadId::Int, uint64_t> buckets;

    // Inspect the buckets to find candidates.
    void inspectBuckets(size_t threadId);

    // Data structure used to store candidate pairs.
    // Indexed by readId0, the read id of the lower numbered
    // read in the pair.
    class Candidate {
    public:
        ReadId readId1;      // The higher numbered read in the pair, readId1 > readId0.
        uint16_t frequency;  // Number of times this pair was found during MinHash.
        bool isSameStrand;   // True if the two reads are on the same strand.
        Candidate(ReadId readId1, uint16_t frequency, bool isSameStrand) :
            readId1(readId1), frequency(frequency), isSameStrand(isSameStrand) {}
    };
    vector< vector<Candidate> > candidates;

    // The total number of candidate found so far, as seen by each thread.
    // This only counts candidates with frequency
    // at least equal to minFrequency.
    vector<size_t> totalCandidateCountByThread;
};

#endif
