#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include "snake_test.h"


using namespace std;

#define PORT 6335
#define IP_SERVER "127.0.0.1"
   
void printPlayers(vector<Player> players)
{
    system("clear");

    for (int i=0; i<players.size(); i++)
    {
        cout << "###############################################################" << endl;
        cout << " Player " << endl;
        cout << "Nome: " << players[i].Name << endl;
        cout << "Score: " << players[i].PlayerScore << endl;
        cout << "Id: " << players[i].id << endl;
        cout << "Direction: " << players[i].S_Dir << endl;
        cout << "Head: [" << players[i].head.x << " , " <<  players[i].head.y << "]" << endl;
    }

    return;
}

int main() 
{
    // variaveis jogo
    Player player;
    int countPlayers;
    bool Gameover = false;
    bool Startgame = false;

    // variaveis socket
    int s0, f, length;	
    struct sockaddr_in server;
    int rval, i;
    char c ;

    s0=socket(AF_INET, SOCK_STREAM, 0);
    if (s0<0)
    {
            perror("opening stream socket");
            exit(1);
    }

    //conexao com o server
    bzero(&server, sizeof(server)) ;   
    server.sin_family = AF_INET ;
    server.sin_port = htons (PORT) ;
    server.sin_addr.s_addr = inet_addr(IP_SERVER); 

    connect(s0, (struct sockaddr *)&server, sizeof (server));
    
    cout << "Digite seu Nome: ";
    cin >> player.Name;
    player.head.x = WIDTH/2;
    player.head.y = HEIGHT/2;
    player.S_Dir = STOP;

    cout << "Solicitando servico ao servidor "<< endl;
    rval = send(s0, &player, sizeof(player), 0);

    while (!Startgame)
    {
        cout << " Esperando o jogo comecar do lado do cliente" << endl;
        rval = recv(s0, &Startgame, sizeof(Startgame), 0);
        rval = recv(s0, &countPlayers, sizeof(countPlayers), 0);
        cout << "Startgame: " << Startgame << endl;
        cout << "countPlayers: " << countPlayers << endl;
        sleep(5);
    }

    rval = recv(s0, &Gameover, sizeof(bool), 0);

    while( !Gameover )
    {
        //system("clear");
        cout << " Comecando jogo do lado do cliente" << endl;
        //recebe jogador
        rval = recv(s0, &player, sizeof(player), 0);
        
        //cout << "-----------------------" << endl;
        //cout << "Player " << i << endl;
        //cout << "Id: " << player.id << endl;
        //cout << "Nome: " << player.Name << endl;
        //cout << "Score: " << player.PlayerScore << endl;
        //cout << "Direction: " << player.S_Dir << endl;
        //cout << "Head: [" << player.head.x << " , " << player.head.y << "]" << endl;
        //sleep(10);
    }

    close (s0);

}
