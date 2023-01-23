#include "Utils/parseFlags.hpp"

typedef std::function<void(inputFlags&)> NoArgHandle;
typedef std::function<void(inputFlags&, const std::string&)> OneArgHandle;

const std::unordered_map<std::string, NoArgHandle> NoArgs{
  {"--help", [](inputFlags& s) { s.help = true; }},
  {"-h", [](inputFlags& s) { s.help = true; }},

  // Verbose mode, will save the mesh at each step and output more
  //  info to console
  {"--verbose", [](inputFlags& s) { s.verbose = true; }},
  {"-v", [](inputFlags& s) { s.verbose = true; }},

  {"--quiet", [](inputFlags& s) { s.verbose = false; }},
};

const std::unordered_map<std::string, OneArgHandle> OneArgs {
  {"-o", [](inputFlags& s, const std::string& arg) {s.outfile = arg;}},
  {"--output", [](inputFlags& s, const std::string& arg) {s.outfile = arg;}},

  {"-i", [](inputFlags& s, const std::string& arg) {s.infile = arg;}},
  {"--input", [](inputFlags& s, const std::string& arg) {s.infile = arg;}},

  {"-p", [](inputFlags& s, const std::string& arg) {s.order = std::stoi(arg);}},
  {"--order", [](inputFlags& s, const std::string& arg) {s.order = std::stoi(arg);}},

  {"--max_tri", [](inputFlags& s, const std::string& arg) {s.max_tri_area = std::stod(arg);}},

  {"--max_tet", [](inputFlags& s, const std::string& arg) {s.max_tet_vol = std::stod(arg);}},
};

inputFlags parse_settings(int argc, const char* argv[]) {
  // inputFlags object we will return
  inputFlags settings;

  // Start at one because arg 0 is the program name 
  for(int i {1}; i < argc; i++) {
    std::string opt {argv[i]};

    // Is this a NoArg?
    if(auto j {NoArgs.find(opt)}; j != NoArgs.end())
    {
      j->second(settings); // Yes, handle it!
    }
      

    // Is this a OneArg?
    else if(auto k {OneArgs.find(opt)}; k != OneArgs.end())
    {
      // Yes, do we have a parameter?
      if(++i < argc)
      {
        // Yes, handle it!
        k->second(settings, {argv[i]});
      }
        
      else
      {
        // No, and we cannot continue, throw an error
        throw std::runtime_error {"missing param after " + opt};
      }
    }    


      
      

    // Yes, possibly throw here, or just print an error
    else
      std::cerr << "unrecognized command-line option " << opt << std::endl;
  }
      
  if(!settings.infile)
  {
    // No, use this as the input file
    throw std::runtime_error {"missing input mesh paramter!"};
  }

  return settings;
}

void inputFlags::setSwitches()
{

  if(order == 2)
  {
    tetSettings += "o2";
    triSettings += "o2";
  }
  else if(order > 2)
  {
    throw std::runtime_error {"Please set order to 1 or 2 (if left blank\
    first order is used by default)"};
  }

  // If verbose 
  if(verbose)
  {
    tetSettings += "V";
    triSettings += "V";
  }
  else
  {
    tetSettings += "Q";
    triSettings += "Q";
  }

  if(max_tet_vol.has_value())
  {
    tetSettings += "a" + std::to_string(max_tet_vol.value());
  }

  if(max_tri_area.has_value())
  {
    triSettings += "a" + std::to_string(max_tri_area.value());
  }

  
}