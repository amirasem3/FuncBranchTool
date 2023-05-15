#include "pin.H"
#include "network.h"
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int server_fd;

VOID BranchCallBack(VOID *source, VOID *target, bool taken){
  FuncBranchInfo info;
  info.type  = MessageType::BRANCH_INFO;
  info.message.branchInfo.sourceAddr = (uint64_t)source;
  info.message.branchInfo.targetAddr = (uint64_t)target;
  info.message.branchInfo.taken = taken;

  send_data(server_fd, &info);
}
VOID RecordBranch(INS ins, VOID *v){
   if(INS_IsBranch(ins)){
      INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)BranchCallBack,
          IARG_INST_PTR,
          IARG_BRANCH_TARGET_ADDR,
          IARG_BRANCH_TAKEN,
          IARG_END);
    }
  }

VOID Routine(RTN rtn, VOID *v){
  RTN_Open(rtn);
  const char *funcName = RTN_Name(rtn).c_str();

  FuncBranchInfo info;
  info.type = MessageType::FUNC_CALL;
  strncpy(info.message.funcInfo.funcName, funcName, 256);
  info.message.funcInfo.funcAddr = (uint64_t)RTN_Address(rtn);

  send_data(server_fd, &info);

  RTN_Close(rtn);
}


VOID Fini(INT32 code, VOID *v){
  close_connection(server_fd);
}

int main(int argc, char *argv[]){
  if(PIN_Init(argc, argv)){
    return 1;
  }
  server_fd = setup_server(8080);
  RTN_AddInstrumentFunction(Routine, 0);
  INS_AddInstrumentFunction(RecordBranch, 0);
  PIN_AddFiniFunction(Fini, 0);
  PIN_StartProgram();
  
  return 0;

}
