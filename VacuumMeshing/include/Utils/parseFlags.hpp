#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct inputFlags {
  bool help = false;
  // be verbose when calling libIGL options, print out all
  bool verbose = false;
  // Length of the cubic boundary
  std::optional<double> boundLen;
  //  The number of subdivisions  (triangular elements) along the side of the
  //  mesh boundary
  std::optional<int> boundSubd = 20;
  // The mesh file to be read in
  std::optional<std::string> infile;
  // The name of the file to be output
  std::optional<std::string> outfile;
  // Order of the mesh
  int order{1};
  // Maximum area of tri elements on bound
  std::optional<double> maxTriArea;
  // Maximum volume of tet elements generated
  std::optional<double> maxTetVol;
  // Command line switches used by the triangle library
  std::string triSettings = "qYa";
  // Command line switches used by the tetgen library
  std::string tetSettings = "pqCYY";

  // Sideset ID's for coil boundary generation
  int coil_sideset_one_id = 1;

  int coil_sideset_two_id = 2;

  // Scaling factors for bounding box generation, default is 120% scaling on all axes
  double scale_factor_x = 1.2;
  double scale_factor_y = 1.2;
  double scale_factor_z = 1.2;

  void setSwitches();
  // Method to print help message
  void helpMessage();

  enum BoundaryType{
    CUBE,
    BOUNDING_BOX
  };

  BoundaryType bound_type = CUBE;

  std::unordered_map<std::string, BoundaryType> boundMap{{"Cube", CUBE}, {"BBox", BOUNDING_BOX}};
};

// Function that will return an inputFlags struct from which we can access CLI
// flags
inputFlags parse_settings(int argc, const char *argv[]);

// Method to print out help message
