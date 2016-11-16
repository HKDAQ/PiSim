#include "../Unity.cpp"

Tool* Factory(std::string tool){
Tool* ret=0;

// if (tool=="Type") tool=new Type;
if (tool=="DummyTool") ret=new DummyTool;


if (tool=="EBUSNTrigger") ret=new EBUSNTrigger;
if (tool=="EBUFEEReceiverSim") ret=new EBUFEEReceiverSim;
if (tool=="EBUDataConverter") ret=new EBUDataConverter;
  if (tool=="EBUNetworkSend") ret=new EBUNetworkSend;
return ret;
}

