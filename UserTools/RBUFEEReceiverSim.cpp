#include "RBUFEEReceiverSim.h"

RBUFEEReceiverSim::RBUFEEReceiverSim():Tool(){}


bool RBUFEEReceiverSim::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  srand(time(NULL));

  m_variables.Get("SleepTime",sleeptime);
  m_variables.Get("Cards",cards);
  m_variables.Get("Channels", channels);
  m_variables.Get("Prob", prob);
  m_variables.Get("BufferSize",buffersize);

  return true;
}


bool RBUFEEReceiverSim::Execute(){
  // long tt=mtime();
  usleep(sleeptime);

  m_data->Data= new FEEBunch();

  for(int card=0; card<cards; card++){
    FEEData* datatmp= new FEEData();
    for(int channel=0; channel<channels; channel++){
      if((rand() % 100)<prob){
	
	FEEChData* tmp=new FEEChData();
	for(int i=0;i<buffersize;i++){
	  tmp->Data.push_back(rand() % 1000);
	}
	tmp->Time=mtime();
	tmp->ChannelID=channel;
	tmp->CardID=card;
	datatmp->Data.push_back(tmp);
	datatmp->Time=tmp->Time;
      }
    }
    m_data->Data->Data.push_back(datatmp);
    m_data->Data->Time=datatmp->Time;
  }
  
  std::cout<<"in sim "<< m_data->Data<<" : "<<m_data->Data->Time<<std::endl;
  // std::cout<<"e1 "<<mtime()-tt<<std::endl;
  return true;
}


bool RBUFEEReceiverSim::Finalise(){

  return true;
}
