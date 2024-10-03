#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "snake_test.h"

#define PORT 6335
#define HEIGHT 22
#define WIDTH  42

using namespace std;

// variaveis jogo
Snake_Game Game;

mutex mtx;

void Playgame()
{
    bool start = false;
    bool gameover = false;

    while ( !start )
    {
        //wait
        cout << " AGUARDANDO JOGADORES! " << endl;

        mtx.lock();
        start = Game.Startgame;
        if( Game.countPlayers >= 2 )
        {
            Game.Startgame = true;    
        }
        mtx.unlock();
    }
    
    cout << "COMECOU O JOGO! " << endl;

    while ( !gameover )
    {
        //wait
        cout << "   JOGANDO! " << endl;
        mtx.lock();
        gameover = Game.GameOver;
        mtx.unlock();
    }
    
}

void waitStart(int Socket)
{
    int rval;

    mtx.lock();
    int count = Game.countPlayers;
    bool start = Game.Startgame;
    bool gameover = Game.GameOver;
    mtx.unlock();

    cout << " Esperando o jogo comecar! " << endl;

    while ( !start )
    {
        //cout << " aguardando " << endl;
        rval = send(Socket, &Game.Startgame, sizeof(Game.Startgame), 0);
        rval = send(Socket, &Game.countPlayers, sizeof(Game.countPlayers), 0);
        mtx.lock();
        start = Game.Startgame;
        count = Game.countPlayers;
        mtx.unlock();    

        if ( !start )
        {
            sleep(5);
        }
        
    }

    rval = send(Socket, &Game.GameOver, sizeof(Game.GameOver), 0);
}

void waitOver(int Socket)
{
    cout << " Esperando o jogo terminar! " << endl;
}

void handleClient(int Socket)
{
    //varaveis jogo
    int id;
    int rval;
    Player newPlayer;
    int count;

    // Receive the data
    rval = recv(Socket, &newPlayer, sizeof(Player), 0);

    mtx.lock();
    Game.setPlayer(newPlayer);
    id = Game.countPlayers--;
    mtx.unlock();

    waitStart(Socket);

    waitOver(Socket);

}

int main()
{      
    //variaveis jogo
    thread Game_thread(Playgame);
    Game_thread.detach();

    //variaveis sockets
    vector<thread> Clients;
    vector<int> Sockets;
    int sock, length;                                          
    struct sockaddr_in server;                                  
    int rval, i, s0;                                          
    char c ;
                                                            
    /* Create socket. */                  
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) 
    {                                            
        perror("opening stream socket");                 
        exit(1);                                           
    }   

    /* Conecta o socket a porta */
    bzero(&server, sizeof(server));                       
    server.sin_family = AF_INET; 
    server.sin_addr.s_addr = INADDR_ANY; 
    server.sin_port = htons (PORT);
    length = sizeof (server);
    bind(sock, (struct sockaddr *)&server, length);

    /*Aguarda conexoes do cliente */    
    listen(sock,5);

    cout << "Servidor aguardando!! "<< endl ;
    while (true) 
    {
        s0 = accept(sock,(struct sockaddr *)0,0);
        Sockets.push_back(s0);
        Clients.emplace_back(handleClient, Sockets.back());
    }

    Game_thread.join();

    // retorna as threads
    for (int i=0; i< Clients.size(); i++)
    {
        Clients[i].join();
    }

    for (int i=0; i< Sockets.size(); i++)
    {
        close(Sockets[i]);
    }
}

