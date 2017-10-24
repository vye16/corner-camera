#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H


#include <cstdio>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <cstring>


class ConfigParser
{
  public:
    ConfigParser(std::string fname);

    void addNumericParam(std::string pname, double default_val);
    void addStringParam(std::string pname, std::string default_val);

    void parse();

    double getNumericParam(std::string pname);
    std::string getStringParam(std::string pname);

  private:
    std::string fname_;

    std::map<std::string, double> num_params_;
    std::map<std::string, std::string> str_params_;
};

#endif
