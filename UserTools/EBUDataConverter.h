#ifndef EBUDataConverter_H
#define EBUDataConverter_H

#include <string>
#include <iostream>

#include "Tool.h"

class EBUDataConverter: public Tool {


 public:

  EBUDataConverter();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:





};


#endif
