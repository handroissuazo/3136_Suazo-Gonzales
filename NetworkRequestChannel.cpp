//
// Created by alejandro on 12/7/15.
//

#include "NetworkRequestChannel.h"

using namespace std;

//NO MORE PIPES

/*--------------------------------------------------------------------------*/
/* CONSTRUCTOR/DESTRUCTOR FOR CLASS   R e q u e s t C h a n n e l  */

//Client side constructor
NetworkRequestChannel::NetworkRequestChannel(const std::string server_host_name, const unsigned short _port_no){

    my_side = CLIENT_SIDE;

    string port = std::to_string(_port_no);
    // first, load up address structs with getaddrinfo():

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int status;

    //getaddrinfo("www.example.com", "3490", &hints, &res);
    if ((status = getaddrinfo(server_host_name.c_str(), port.c_str(), &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return ;
    }

    // make a socket:
    new_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (new_fd < 0)
    {
        perror ("Error creating socket\n");
        return ;
    }

    // connect!
    if (connect(new_fd, res->ai_addr, res->ai_addrlen)<0)
    {
        perror ("connect error\n");
        return ;
    }
    printf ("Successfully connected to the server %s\n", server_host_name.c_str());
}

// Server side constructor
NetworkRequestChannel::NetworkRequestChannel(const unsigned short _port_no, void * (*connectionHandler) (int *)){
      // listen on sock_fd, new connection on new_fd

    my_side = SERVER_SIDE;
//    char s[INET6_ADDRSTRLEN];


    string port = to_string(_port_no);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    while(1) {
        if (make_socket(port) == -1){
            perror("Did not make socket");
            continue;
        }
        if (bind(sockfd, serv->ai_addr, serv->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            port = "0";
            continue;
        }
        else {
            printf("Socket made successfully\n");
            break;
        }
    }

    socklen_t len = serv->ai_addrlen;
    if (getsockname(sockfd, serv->ai_addr, &len) == -1) {
        perror("getsockname");
        return;
    }

    printf("port is %d\n",ntohs(get_in_port(serv->ai_addr)));
    portNum = ntohs(get_in_port(serv->ai_addr));

    freeaddrinfo(serv); // all done with this structure

    if (listen(sockfd, 20) == -1) {
        perror("listen");
        exit(1);
    }
}

void NetworkRequestChannel::WaitForClientInitConnection(){
    socklen_t sin_size;
    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);
        if (new_fd == -1) {
            //Nothing to accept
            continue;
        }
        else{
            break;
        }
    }
    printf("New NetworkRequestChannel waiting for connections...\n");
}

NetworkRequestChannel::~NetworkRequestChannel() {
    if (my_side == SERVER_SIDE) {
        close(new_fd);
        close(sockfd);
    }
    else if (my_side == CLIENT_SIDE){
        close(new_fd);
    }
}

/*--------------------------------------------------------------------------*/
/* READ/WRITE FROM/TO REQUEST CHANNELS  */
/*--------------------------------------------------------------------------*/

const int MAX_MESSAGE = 255;

string NetworkRequestChannel::send_request(string _request) {
    cwrite(_request);
    string s = cread();
    return s;
}

string NetworkRequestChannel::cread() {
    char buf[1024];
    if (new_fd == -1) {
        //Nothing to accept
        perror("Socket is broken");
    }
    else {
        //Something got through!
        recv(new_fd, buf, sizeof(buf), 0);
        printf("received msg: %s\n", buf);
    }

    return buf;
}

int NetworkRequestChannel::cwrite(string _msg) {

    if (send(new_fd, _msg.c_str(), strlen(_msg.c_str())+1, 0) == -1)
    {
        perror("send");
    }

}

int NetworkRequestChannel::make_socket(string port){
    int rv;

    if ((rv = getaddrinfo(NULL, port.c_str(), &hints, &serv)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }

    if ((sockfd = socket(serv->ai_family, serv->ai_socktype, serv->ai_protocol)) == -1) {
        perror("server: socket");
        return -1;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
/* ACCESS THE NAME OF REQUEST CHANNEL  */
/*--------------------------------------------------------------------------*/

string NetworkRequestChannel::port() {
    return to_string(portNum);
}
//
///*--------------------------------------------------------------------------*/
///* ACCESS FILE DESCRIPTORS OF REQUEST CHANNEL  */
///*--------------------------------------------------------------------------*/
//
//int NetworkRequestChannel::read_fd() {
//    return rfd;
//}
//
//int NetworkRequestChannel::write_fd() {
//    return wfd;
//}

//Helpers
in_port_t NetworkRequestChannel::get_in_port(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return (((struct sockaddr_in*)sa)->sin_port);
    }

    return (((struct sockaddr_in6*)sa)->sin6_port);
}
