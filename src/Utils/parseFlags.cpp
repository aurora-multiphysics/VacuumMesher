#include "Utils/parseFlags.hpp"

typedef std::function<void(inputFlags&)> NoArgHandle;
typedef std::function<void(inputFlags&, const std::string&)> OneArgHandle;

const std::unordered_map<std::string, NoArgHandle> NoArgs{
  {"--help", [](inputFlags& s) { s.help = true; }},
  {"-h", [](inputFlags& s) { s.help = true; }},

  {"--verbose", [](inputFlags& s) { s.verbose = true; }},
  {"-v", [](inputFlags& s) { s.verbose = true; }},

  {"--quiet", [](inputFlags& s) { s.verbose = false; }},
};

const std::unordered_map<std::string, OneArgHandle> OneArgs {
  // Writing out the whole lambda
  {"-o", [](inputFlags& s, const std::string& arg) {s.infile = arg;}},
  {"--output", [](inputFlags& s, const std::string& arg) {s.infile = arg;}},

  {"-i", [](inputFlags& s, const std::string& arg) {s.infile = arg;}},
  {"--input", [](inputFlags& s, const std::string& arg) {s.infile = arg;}},

  {"-p", [](inputFlags& s, const std::string& arg) {s.order = std::stoi(arg);}},
};

inputFlags parse_settings(int argc, const char* argv[]) {
  inputFlags settings;

  // Start at one because arg 0 is the program name 
  for(int i {1}; i < argc; i++) {
    std::string opt {argv[i]};

    // Is this a NoArg?
    if(auto j {NoArgs.find(opt)}; j != NoArgs.end())
    {
      j->second(settings); // Yes, handle it!
    }
      

    // No, how about a OneArg?
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

generationSettings::readFlags(inputFlags& flags)
{

  if(flags.order == 2)
  {
    this->tetSettings += "o2";
    this->triSettings += "o2";
  }

  if(flags.verbose)
  {
    this->tetSettings += "V";
    this->triSettings += "V";
  }
  else
  {
    this->tetSettings += "Q";
    this->triSettings += "Q";
  }
}