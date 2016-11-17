#include "BrokerSimulatedTriggerDecision.h"

BrokerSimulatedTriggerDecision::BrokerSimulatedTriggerDecision():Tool(){}


bool BrokerSimulatedTriggerDecision::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  m_data->StartSlicetime=(long)time(NULL);

  return true;
}


bool BrokerSimulatedTriggerDecision::Execute(){

  long currenttime=time(NULL);

  for(long time=currenttime-100; time>=m_data->StartSlicetime; time--){

    m_data->TrigData[time].Decision=((rand() % 100)>50);

    //if true send to event builder
    //if faluse send to EBU
    m_data->TrigData.erase(time);
    //m_data->TrigData. remove form map

  }


  return true;
}


bool BrokerSimulatedTriggerDecision::Finalise(){

  return true;
}
