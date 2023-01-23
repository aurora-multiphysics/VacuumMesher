#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct inputFlags {
    bool help {false};
    // be verbose when calling libIGL options, print out all    
    bool verbose {false};
    // Length of the cubic boundary   
    std::optional<double> bound_len;
    //  The number of subdivisions  (triangular elements) along the side of the mesh boundary 
    std::optional<double> bound_subd {20};
    // The mesh file to be read in
    std::optional<std::string> infile;
    // The name of the file to be output
    std::optional<std::string> outfile;
    // Order of the mesh
    int order {1};
    // Maximum area of tri elements on bound
    std::optional<double> max_tri_area;
    // Maximum volume of tet elements generated
    std::optional<double> max_tet_vol;

};

struct generationSettings {

    std::string triSettings {"qYa"};
    std::string tetSettings {"pqCYY"};

    void readFlags(inputFlags& flags);
};

// Function that will return an inputFlags struct from which we can access CLI flags
inputFlags parse_settings(int argc, const char* argv[]);
