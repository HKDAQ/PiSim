#include "EBUNetworkSend.h"

EBUNetworkSend::EBUNetworkSend():Tool(){}


bool EBUNetworkSend::Initialise(std::string configfile, DataModel &data){

  if(configfile!="")  m_variables.Initialise(configfile);
  //m_variables.Print();

  m_data= &data;

  //setup and connect worker threads

  Isend = new zmq::socket_t(*(m_data->context), ZMQ_PAIR);
  Isend->bind("inproc://main2processor");

  args=new context_args(m_data->context);
     
  pthread_create (&thread[0], NULL, EBUNetworkSend::ProcessorThread, args);
  pthread_create (&thread[1], NULL, EBUNetworkSend::TriggerThread, args);
  pthread_create (&thread[2], NULL, EBUNetworkSend::FullDataThread, args);

  sleep(5);
  return true;
}


bool EBUNetworkSend::Execute(){

  for(int i=0; i<m_data->Data.size(); i++){

    //throw data to worker thread
    
    std::stringstream Datapointer;
    Datapointer<<"D "<<m_data->Data.at(i);

    zmq::message_t message(Datapointer.str().length()+1);
    snprintf ((char *) message.data(), Datapointer.str().length()+1 , "%s" ,Datapointer.str().c_str()) ;
    Isend->send(message);
    
    //throw data to worker thread
    m_data->Data.at(i)=0;
  }

  m_data->Data.clear();
  
  return true;
}


bool EBUNetworkSend::Finalise(){

  zmq::message_t message(5);
  snprintf ((char *) message.data(),5 , "%s" ,"Quit") ;
  Isend->send(message);

  std::cout<<"main send quit"<<std::endl;
  pthread_join(thread[0], NULL);
  std::cout<<"join 0"<<std::endl;
  pthread_join(thread[1], NULL);
  std::cout<<"join 1"<<std::endl;
  pthread_join(thread[2], NULL);
  std::cout<<"join 2"<<std::endl;

  delete args;
  args=0;

  delete Isend;
  Isend=0;
  
  return true;
}

//worker thread sends time and id then buffers

void* EBUNetworkSend::ProcessorThread(void* arg){

  //std::cout<<"debug 1"<<std::endl;
  
  context_args* args= static_cast<context_args*>(arg);
  
  zmq::socket_t Ireceive (*(args->context), ZMQ_PAIR);
  Ireceive.connect("inproc://main2processor");
  
  zmq::socket_t trigsend(*(args->context), ZMQ_PAIR);
  trigsend.bind("inproc://processor2trigger");
  
  zmq::socket_t fulldatasend(*(args->context), ZMQ_PAIR);
  fulldatasend.bind("inproc://processor2fulldata");
  
  bool running=true;

  //std::cout<<"debug 2"<<std::endl;
  
  while (running){

    //std::cout<<"debug 3"<<std::endl;
    
    zmq::message_t comm;
    Ireceive.recv(&comm);
    
    std::istringstream iss(static_cast<char*>(comm.data()));
    std::string arg1="";
    long long unsigned int arg2;
    TTree *tree;
    
    iss>>arg1;
    
    if (arg1=="D"){
      iss>>std::hex>>arg2;
      
      //std::cout<<"debug 4"<<std::endl;
      ////do processing if still neeed

      FEEData *data;
      data=reinterpret_cast<FEEData *>(arg2);

      std::stringstream tmp;
      tmp<< "D "<<data->Time<<" "<<data->CardID<<" "<<data->ChannelID;
      zmq::message_t message(tmp.str().length()+1);
      snprintf ((char *) message.data(), tmp.str().length()+1 , "%s" ,tmp.str().c_str()) ;
      //std::cout<<"debug 4.0"<<std::endl;
      trigsend.send(message);
      //std::cout<<"debug 4.1"<<std::endl;
      fulldatasend.send(comm);
      
    }
    
    else if(arg1=="Quit")running=false;
    
  }

  //std::cout<<"debug 5"<<std::endl; 
  
  zmq::message_t message(5);
  snprintf ((char *) message.data(),5 , "%s" ,"Quit");
  trigsend.send(message);
  snprintf ((char *) message.data(),5 , "%s" ,"Quit");
  fulldatasend.send(message);

  //std::cout<<"debug 6"<<std::endl;
  
  pthread_exit(NULL);

}


void* EBUNetworkSend::TriggerThread(void* arg){
  
  context_args* args= static_cast<context_args*>(arg);
  
  zmq::socket_t Ireceive (*(args->context), ZMQ_PAIR);
  Ireceive.connect("inproc://processor2trigger");
  
  zmq::socket_t indexsend(*(args->context), ZMQ_PAIR);
  indexsend.bind("inproc://trigger2fulldata");
  
  bool running=true;
  
  while(running){
    
    zmq::message_t comm;
    Ireceive.recv(&comm);
    
    std::istringstream iss(static_cast<char*>(comm.data()));
    std::string arg1="";
    long long unsigned int arg2;
    
    
    iss>>arg1;
    
    if (arg1=="D"){
      //probably buffer input and poll out buffer to broker
      FEEData data;
      iss>>data.Time>>data.CardID>>data.ChannelID;  
      //std::cout<<data.Time<<std::endl;
      //send it
      
    }
    
    else if(arg1=="Quit")running=false;
    
  }
  
  pthread_exit(NULL);
  
}

void* EBUNetworkSend::FullDataThread(void* arg){
  
  context_args* args= static_cast<context_args*>(arg);
  
  zmq::socket_t Ireceive (*(args->context), ZMQ_PAIR);
  Ireceive.connect("inproc://processor2fulldata");
  
  zmq::socket_t indexreceive (*(args->context), ZMQ_PAIR);
  indexreceive.connect("inproc://trigger2fulldata");
  
  bool running=true;
  
  while(running){
    
    zmq::message_t comm;
    Ireceive.recv(&comm);
    
    std::istringstream iss(static_cast<char*>(comm.data()));
    std::string arg1="";
    long long unsigned int arg2;
   
    std::deque<FEEData*> waveforms;
    
    iss>>arg1;
    
    if (arg1=="D"){
      iss>>std::hex>>arg2;
      
      ////do processing if still neeed

      FEEData *data;
      data=reinterpret_cast<FEEData *>(arg2);
      waveforms.push_back(data);
      
    }
    
    else if(arg1=="Quit")running=false;
    
  }
  
  pthread_exit(NULL);
}
