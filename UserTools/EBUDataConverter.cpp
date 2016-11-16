#include "EBUDataConverter.h"

EBUDataConverter::EBUDataConverter():Tool(){}


bool EBUDataConverter::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  return true;
}


bool EBUDataConverter::Execute(){

  return true;
}


bool EBUDataConverter::Finalise(){

  return true;
}
