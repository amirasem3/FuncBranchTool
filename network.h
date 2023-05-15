#ifndef NETWORK_H
#define NETWORK_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <iostream>
#include <unistd.h>

enum MessageType {
  FUNC_CALL = 0,
  BRANCH_INFO = 1,
  END_OF_DATA = 2
};


union Message{
  struct{
    uint64_t sourceAddr;
    uint64_t targetAddr;
    bool taken;
  }branchInfo;
 struct{
   char funcName[256];
   uint64_t funcAddr;
 }funcInfo;
};

struct FuncBranchInfo{
  MessageType type;
  Message message;
};

int new_socket = 0;

static int setup_server(int port){
  int server_fd;
  struct sockaddr_in address;

  if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    std::cout << "socket creation failed"  << std::endl;
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
    std::cout << "bind failed" << std::endl;
  }
  if(listen(server_fd, 3) < 0){
    std::cout <<"listen failed" << std::endl;
  }

  return server_fd;
}

static void send_data(int server_fd, FuncBranchInfo* info){
  static bool connection_open = false;
  if(!connection_open){
  int addrlen = sizeof(struct sockaddr_in);
  struct sockaddr_in address;
  
  if((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0 ){
    std::cout <<"Accept failed" << std::endl;
  }
  connection_open = true;
  }
  send(new_socket, info, sizeof(*info), 0);
}

static void close_connection(int server_fd){
  FuncBranchInfo info;
  info.type = MessageType::END_OF_DATA;
  send_data(server_fd, &info);

  close(new_socket);
  new_socket = 0;
}

#endif
