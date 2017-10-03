#include "ConfigParser.h"



ConfigParser::ConfigParser(std::string fname) : fname_(fname)
{
}


void ConfigParser::addNumericParam(
    std::string pname, double default_val)
{
  num_params_[pname] = default_val;
}


double ConfigParser::getNumericParam(std::string pname)
{
  std::map<std::string, double>::iterator it =
    num_params_.find(pname);
  if (it == num_params_.end()) {
    printf("%s is not a defined parameter\n", pname.c_str());
    throw std::runtime_error("config parsing error");
  }
  return it->second;
}


void ConfigParser::addStringParam(
    std::string pname, std::string default_val)
{
  str_params_[pname] = default_val;
}


std::string ConfigParser::getStringParam(std::string pname)
{
  std::map<std::string, std::string>::iterator it = 
    str_params_.find(pname);
  if (it == str_params_.end()) {
    printf("%s is not a defined parameter\n", pname.c_str());
    throw std::runtime_error("config parsing error");
  }
  return it->second;
}


std::string trim(std::string str, std::string whitespace=" \t")
{
  size_t start = str.find_first_not_of(whitespace);
  if (start == std::string::npos) {
    return "";
  }
  size_t range = str.find_last_not_of(whitespace) - start + 1;
  return str.substr(start, range);
}


void ConfigParser::parse()
{
  if (strcmp(fname_.c_str(), "") == 0) {
    return;
  }

  std::ifstream infile(fname_);
  if (!infile) {
    printf("ERROR: cannot open config file %s.\n", fname_.c_str());
    return;
  }
  printf("Reading params from config file %s.\n", fname_.c_str());

  std::string curline, key, val;
  int comment, pos, line;
  while (std::getline(infile, curline)) {
    // cut out commented parts
    comment = curline.find_first_of("#");
    curline = trim(curline.substr(0, comment));
    if (curline == "")
      continue;

    if ((pos = curline.find("=")) == std::string::npos) {
      printf("line %d does not properly define a parameter", line);
      throw std::runtime_error("config parsing error");
    }

    key = trim(curline.substr(0, pos));
    curline.erase(0, pos + 1);
    val = trim(curline);

    // find key in our param maps
    if (num_params_.find(key) != num_params_.end()) {
      num_params_[key] = std::atof(val.c_str());
      printf("- setting parameter %s to double value %f\n",
          key.c_str(), num_params_[key]);
    } else if (str_params_.find(key) != str_params_.end()) {
      str_params_[key] = val;
      printf("- setting parameter %s to string value %s\n",
          key.c_str(), str_params_[key].c_str());
    } else {
      printf("ERROR: undefined parameter %s cannot be set to %s\n",
          key.c_str(), val.c_str());
      throw std::runtime_error("config parsing error");
    }

    line++;
  }
}
