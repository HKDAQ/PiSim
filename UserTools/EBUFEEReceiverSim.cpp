#include "EBUFEEReceiverSim.h"

EBUFEEReceiverSim::EBUFEEReceiverSim():Tool(){}


bool EBUFEEReceiverSim::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  srand(time(NULL));
  return true;
}


bool EBUFEEReceiverSim::Execute(){

  sleep(1);

  for(int card=0; card<20; card++){
    for(int channel=0; channel<20; channel++){
      if((rand() % 100)>20){
	
	FEEData* tmp=new FEEData();
	for(int i=0;i<1000;i++){
	  tmp->Data.push_back(rand() % 1000);
	}
	tmp->Time=(long)time(NULL);
	tmp->ChannelID=channel;
	tmp->CardID=card;
	m_data->Data.push_back(tmp);
      }
    }
  }
  
  return true;
}


bool EBUFEEReceiverSim::Finalise(){

  return true;
}
