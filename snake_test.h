/*
    Definiçoes do jogo 
        - A frutinha sera * 
        - A cabeça da cobrinha sera @ e o corpo sera 0 
        - E as paredes serão #

    Mapa 
        - fruta == -1
        - espaço em branco == 0
        - 1 < player < 5
    
    map2
    -2 -> bordas do jogo
    -1 -> fruta do jogo
     0 -> espaço em branco
     1 -> Body
     2 -> Head
    { 1,2,3,4,5,6 } -> jogadores

*/

#include <iostream>
#include <iomanip>


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <ctype.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

#define HEIGHT 22
#define WIDTH  42

using namespace std;


enum snakesDirection { STOP = 0, UP, DOWN, RIGHT, LEFT }; 

struct Body
{
    int x;
    int y;
};

struct Fruit
{
    int Cord_x;
    int Cord_y;
};

struct Player 
{
    int id;
    char Name[100];
    snakesDirection S_Dir; 
    int PlayerScore=0;
    Body tail[1000];
    Body head;
};

class Snake_Game
{
    public:
        bool Startgame = false;
        bool GameOver = false;
        int countPlayers=0;

    private:                                                                                                         
        vector<vector<int>> Mapa;
        Fruit fruit;
        vector<Player> players;

        void setMap()
        {
            for (int i=0; i <HEIGHT ; i++)
            {
                vector<int> line;
                for (int j=0; j < WIDTH; j++)
                {
                    if( i == 0 || j==0 || i==HEIGHT-1 || j == WIDTH-1 )
                    {
                        line.push_back(-2);
                    }
                    else
                    {
                        line.push_back(0);
                    }
                }
            }
        };

        void setFruit()
        {
            fruit.Cord_x = rand() % WIDTH;
            fruit.Cord_y = rand() % HEIGHT;
            while ( (fruit.Cord_y ==0 || fruit.Cord_y ==HEIGHT-1 ) || ( fruit.Cord_x ==0 || fruit.Cord_x ==WIDTH-1 ))
            {
                fruit.Cord_x = rand() % WIDTH;
                fruit.Cord_y = rand() % HEIGHT;
            }
        }

    public:

        void setPlayer(Player newPlayer)
        {
            newPlayer.id = countPlayers;
            newPlayer.PlayerScore = 0;
            newPlayer.S_Dir = STOP;
            newPlayer.head.x = 0;
            newPlayer.head.y = 0;
            countPlayers++;
        }

        Player getPlayer(int i)
        {
            return players[i];
        }

        vector<vector<int>> getMapa()
        {
            return Mapa;
        }

        void Start()
        {
            setMap();
            setFruit();
            GameOver = false;
        };

        void Logic()
        {

        };

        //DEBUG
        void printPlayers()
        {
            
            for (int i = 0; i < players.size(); i++)
            {
                cout << "-----------------------" << endl;
                cout << "Player " << i << endl;
                cout << "Id: " << players[i].id << endl;
                cout << "Nome: " << players[i].Name << endl;
                cout << "Score: " << players[i].PlayerScore << endl;
                cout << "Direction: " << players[i].S_Dir << endl;
                cout << "Head: [" << players[i].head.x << " , " << players[i].head.y << "]" << endl;
            }
        }
};


void pause_P() 
{
    int a;
    printf("Pressione Enter para continuar...");
    scanf("%d", &a);
}

int kbhit(void) {
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Obtenha as configurações atuais do terminal
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Desabilite a entrada canônica e o modo echo
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Altere as configurações do descritor de arquivos para não-bloqueante
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Tente ler um caractere do stdin
    ch = getchar();

    // Restaure as configurações antigas do descritor de arquivo e do terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // Se nenhum caractere foi lido, retorne 0, caso contrário, retorne 1
    if (ch != EOF) {
        ungetc(ch, stdin); // Coloque o caractere de volta no buffer
        return 1; // Tecla foi pressionada
    }

    return 0; // Nenhuma tecla pressionada
}

void Input()
{
    char c;
    snakesDirection dir;

    if( kbhit() )
    {
        c = getchar();
        c = toupper(c);
        

        switch (c)
        {
            case 'W': 
                dir = UP;
                break;
            case 'S': 
                dir = DOWN;
                break;
            case 'A':
                dir = LEFT; 
                break;
            case 'D': 
                dir = RIGHT;
                break;

            default:
                break;
        }
    
    }
}