#include "EBUSNTrigger.h"

EBUSNTrigger::EBUSNTrigger():Tool(){}


bool EBUSNTrigger::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  return true;
}


bool EBUSNTrigger::Execute(){

  return true;
}


bool EBUSNTrigger::Finalise(){

  return true;
}
