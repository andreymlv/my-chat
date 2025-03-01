#include "client.h"
#include<iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <thread>

#define BUFF_SIZE 1024
using namespace std;

void read_message(int socket){
  char buffer[BUFF_SIZE];
  memset(buffer, 0, BUFF_SIZE);
  while(true){
    memset(buffer, 0, BUFF_SIZE);
    int readed_size = recv(socket, buffer, BUFF_SIZE-1, 0);
    if(readed_size <= 0 ) break;
    cout<<buffer<<endl;
    if(string(buffer) == "Goodbuy!") break;
  }
}


void init_client(const char *ip, const char *port){
  
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(client_socket == -1){
    cerr<<"error_create_socket"<<endl;
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = inet_addr(ip);
  server_address.sin_port = stoi(port);

  if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
    cerr << "connect_error" <<endl;
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  thread thread_read(read_message,client_socket);
  string name;
  getline(cin, name);
  send(client_socket, name.c_str(), name.size(), 0);
  
  while(true){
    string message;
    getline(cin, message);
    if (message == "exit()") {
      send(client_socket, message.c_str(), message.size(), 0);
      break;
    }
    if (message.size() == 0) continue;
    if (send(client_socket, message.c_str(), message.size(), 0)<0){
      cerr<<"error sending message"<<endl;
      break;
    }
  }
  thread_read.join();
  close(client_socket);
}
