#ifndef SHASTA_ASSEMBLER_OPTIONS_HPP
#define SHASTA_ASSEMBLER_OPTIONS_HPP

// Boost libraries.
#include <boost/program_options.hpp>

// Standard library.
#include "iostream.hpp"
#include "string.hpp"
#include "vector.hpp"

namespace shasta {
    class AssemblerOptions;
}



// Class AssemblyOptions contains one nested class
// corresponding to each group of options.
class shasta::AssemblerOptions {
public:

    // Options only allowed on the command line and not in the configuration file.
    class CommandLineOnlyOptions {
    public:
        string configFileName;
        vector <string> inputFastaFileNames;
        string assemblyDirectory;
        string command;
        string memoryMode;
        string memoryBacking;
    };
    CommandLineOnlyOptions commandLineOnlyOptions;



    // Options in the [Reads] section of the configuration file.
    // Can also be entered on the command line with option names
    // beginning with "Reads.".
    class ReadsOptions {
    public:
        int minReadLength;
        class PalindromicReadOptions {
        public:
            int maxSkip;
            int maxMarkerFrequency;
            double alignedFractionThreshold;
            double nearDiagonalFractionThreshold;
            int deltaThreshold;
            void write(ostream&) const;
        };
        PalindromicReadOptions palindromicReads;

        void write(ostream&) const;
    };
    ReadsOptions readsOptions;



    // Options in the [Kmers] section of the configuration file.
    // Can also be entered on the command line with option names
    // beginning with "Kmers.".
    class KmersOptions {
    public:
        int k;
        double probability;
        void write(ostream&) const;
    };
    KmersOptions kmersOptions;



    // Options in the [MinHash] section of the configuration file.
    // Can also be entered on the command line with option names
    // beginning with "MinHash.".
    class MinHashOptions {
    public:
        int m;
        double hashFraction;
        int minHashIterationCount;
        int maxBucketSize;
        int minFrequency;
        void write(ostream&) const;
    };
    MinHashOptions minHashOptions;



    // Options in the [Align] section of the configuration file.
    // Can also be entered on the command line with option names
    // beginning with "Align.".
    class AlignOptions {
    public:
        int maxSkip;
        int maxTrim;
        int maxMarkerFrequency;
        int minAlignedMarkerCount;
        void write(ostream&) const;
    };
    AlignOptions alignOptions;



    // Options in the [ReadGraph] section of the configuration file.
    // Can also be entered on the command line with option names
    // beginning with "ReadGraph.".
    class ReadGraphOptions {
    public:
        int maxAlignmentCount;
        int minComponentSize;
        int maxChimericReadDistance;
        void write(ostream& ) const;
    };
    ReadGraphOptions readGraphOptions;



    // Options in the [MarkerGraph] section of the configuration file.
    // Can also be entered on the command line with option names
    // beginning with "MarkerGraph.".
    class MarkerGraphOptions {
    public:
        int minCoverage;
        int maxCoverage;
        int lowCoverageThreshold;
        int highCoverageThreshold;
        int maxDistance;
        int edgeMarkerSkipThreshold;
        int pruneIterationCount;
        string simplifyMaxLength;
        vector<size_t> simplifyMaxLengthVector;
        void parseSimplifyMaxLength();
        void write(ostream&) const;
    };
    MarkerGraphOptions markerGraphOptions;



    // Options in the [Assembly] section of the configuration file.
    // Can also be entered on the command line with option names
    // beginning with "Assembly.".
    class AssemblyOptions {
    public:
        int strategy;   // 0: bubble removal, 1: phasing
        int crossEdgeCoverageThreshold;
        int markerGraphEdgeLengthThresholdForConsensus;
        string consensusCaller;
        bool useMarginPhase;
        bool storeCoverageData;
        void write(ostream&) const;
    };
    AssemblyOptions assemblyOptions;

    // Constructor.
    AssemblerOptions(int argumentCount, const char** arguments);

    // Add configurable options to the Boost option description object.
    void addCommandLineOnlyOptions();
    void addConfigurableOptions();

    // Write the options as a config file.
    void write(ostream&) const;


    // Boost program_options library objects.
    boost::program_options::options_description commandLineOnlyOptionsDescription;
    boost::program_options::options_description configurableOptionsDescription;
    boost::program_options::options_description allOptionsDescription;

    // This one is the same as allOptionsDescription, with
    // "--invalidOption" added to capture invalid positional options.
    vector<string> invalidPositionalOptions;
    boost::program_options::options_description allOptionsIncludingInvalidDescription;

    // Function to convert a bool to True or False for better
    // compatibility with Python scripts.
    static string convertBoolToPythonString(bool);

};

#endif

