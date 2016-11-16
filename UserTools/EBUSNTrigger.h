#ifndef EBUSNTrigger_H
#define EBUSNTrigger_H

#include <string>
#include <iostream>

#include "Tool.h"

class EBUSNTrigger: public Tool {


 public:

  EBUSNTrigger();
  bool Initialise(std::string configfile,DataModel &data);
  bool Execute();
  bool Finalise();


 private:





};


#endif
