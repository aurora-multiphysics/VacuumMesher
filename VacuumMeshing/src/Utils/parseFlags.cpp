#include "Utils/parseFlags.hpp"

typedef std::function<void(inputFlags &)> NoArgHandle;
typedef std::function<void(inputFlags &, const std::string &)> OneArgHandle;

const std::unordered_map<std::string, NoArgHandle> NoArgs{
    {"--help", [](inputFlags &s) { s.help = true; }},
    {"-h", [](inputFlags &s) { s.help = true; }},

    // Verbose mode, will save the mesh at each step and output more
    //  info to console
    {"--verbose", [](inputFlags &s) { s.verbose = true; }},
    {"-v", [](inputFlags &s) { s.verbose = true; }},

    {"--quiet", [](inputFlags &s) { s.verbose = false; }},
};

const std::unordered_map<std::string, OneArgHandle> OneArgs{
    {"-o", [](inputFlags &s, const std::string &arg) { s.outfile = arg; }},
    {"--output",
     [](inputFlags &s, const std::string &arg) { s.outfile = arg; }},

    {"-i", [](inputFlags &s, const std::string &arg) { s.infile = arg; }},
    {"--input", [](inputFlags &s, const std::string &arg) { s.infile = arg; }},

    {"-p",
     [](inputFlags &s, const std::string &arg) { s.order = std::stoi(arg); }},
    {"--order",
     [](inputFlags &s, const std::string &arg) { s.order = std::stoi(arg); }},

    {"--max_tri",
     [](inputFlags &s, const std::string &arg) {
       s.maxTriArea = std::stod(arg);
     }},

    {"--max_tet", [](inputFlags &s,
                     const std::string &arg) { s.maxTetVol = std::stod(arg); }},

    {"--bound_subdiv",
     [](inputFlags &s, const std::string &arg) {
       s.boundSubd = std::stoi(arg);
     }},

    {"--bound_len",
     [](inputFlags &s, const std::string &arg) {
       s.boundLen = std::stod(arg);
     }},
};

inputFlags parse_settings(int argc, const char *argv[]) {

  // inputFlags object we will return
  inputFlags settings;

  // Start at 1 because arg 0 is the exe name
  for (int i = 1; i < argc; i++) {
    std::string opt = argv[i];
    // Check if this is a noArg
    if (auto j = NoArgs.find(opt); j != NoArgs.end()) {
      // If arg is found, set it's corresponding switch to true
      j->second(settings);
    }

    // Check if this is a OneArg
    else if (auto k = OneArgs.find(opt); k != OneArgs.end()) {
      // Check if the arg has a parameter
      if (++i > argc) {
        // If we enter this conditional then the arg was not given a parameter,
        //  so throw an error
        throw std::runtime_error{"missing param after " + opt};
      }

      // An argument was recieved, use it in settings
      k->second(settings, argv[i]);
    }

    // If the argument is not recognised throw an error and print the
    // unrecognised option
    else {
      std::cerr << "unrecognized command-line option " << opt << std::endl;
    }
  }

  if (settings.help) {
    // Print help message
    settings.helpMessage();
  }

  if (!settings.infile) {
    // No input mesh parameter was given by user, throw an error
    throw std::runtime_error{"missing input mesh paramter!"};
  }
  return settings;
}

void inputFlags::setSwitches() {
  if (order == 2) {
    tetSettings += "o2";
    triSettings += "o2";
  } else if (order > 2) {
    throw std::runtime_error{"Please set order to 1 or 2 (if left blank\
    first order is used by default)"};
  }

  // If verbose
  if (verbose) {
    tetSettings += "V";
    triSettings += "V";
  } else {
    tetSettings += "Q";
    triSettings += "Q";
  }

  if (maxTetVol.has_value()) {
    tetSettings += "a" + std::to_string(maxTetVol.value());
  }

  if (maxTriArea.has_value()) {
    triSettings += "a" + std::to_string(maxTriArea.value());
  }
}

void inputFlags::helpMessage() {
  std::cout
      << "\nVacuumMesher "
      << "Options:" << std::endl
      << "-h | --help        Print this help" << std::endl
      << "-v | --verbose     Print out more info from this program and "
         "contributing libs"
      << std::endl
      << "-i | --input       Specify the input mesh file" << std::endl
      << "-o | --output      Use non-defualt name for " << std::endl
      << "-p | --order       Set desired element order (stuck at 1 for now) "
      << std::endl
      << "--max_tri          Set maximum triangle area " << std::endl
      << "--max_tet          Set maximum tetrahedra volume" << std::endl
      << "--bound_subdiv     Set the number of subdivisions on one edge of the "
         "boundary"
      << std::endl
      << "--bound_len        Set the length of one edge of the boundary\n"
      << std::endl;
}