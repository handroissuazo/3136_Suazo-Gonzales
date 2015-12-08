//
// Created by alejandro on 12/7/15.
//
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef CLIENT_NETWORKREQUESTCHANNEL_H
#define CLIENT_NETWORKREQUESTCHANNEL_H



class NetworkRequestChannel {

public:
    typedef enum {SERVER_SIDE, CLIENT_SIDE} Side;

    Side my_side;

    struct addrinfo hints, *res, *serv; // hints is for both: res is for the client: serv is for the server
    struct sockaddr_storage their_addr; // this is for the server
    int sockfd, new_fd; // sockfd is for both: new_fd is for the server
    int portNum;

    // Client side request channel constructor
    NetworkRequestChannel(const std::string server_host_name, const unsigned short _port_no);

    //Server side request channel constructor (the second argument is the)
    NetworkRequestChannel(const unsigned short _port_no, void * (*connectionHandler) (int *));

    ~NetworkRequestChannel();

    int make_socket(std::string port);

    std::string send_request(std::string _request);

    std::string cread();

    std::string port();

    void WaitForClientInitConnection();
    int cwrite(std::string _msg);

    //Helpers
    in_port_t get_in_port(struct sockaddr *sa);
};


#endif //CLIENT_NETWORKREQUESTCHANNEL_H
