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
    { 1,2,3,4,5,6 } -> jogadores

*/

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


bool GameOver;
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
    string Name;
    snakesDirection S_Dir; 
    int PlayerScore=0;
    vector<Body> tail;
    int Cord_x;
    int Cord_y;
};

class Snake_Game
{
    private:
        int Mapa[HEIGHT][WIDTH];
        Fruit fruit;
        vector<Player> players;
    
        void Set_Fruit()
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

        void Setup_Player(Player p1)
        {
            p1.PlayerScore=0;
            p1.S_Dir = STOP;
            players.push_back(p1);
        };

        void Setup()
        {
            GameOver = false;
            players[0].Cord_x = WIDTH  / 2;
            players[0].Cord_y = HEIGHT / 2;
            Set_Fruit();
        };

        void Draw()
        {
            system("clear");

            //top corner
            for (int i=0; i<WIDTH ; i++)
            {
                cout << GREEN << "#" << RESET;
            }cout << endl;

            //midle of the board
            for (int i=0; i<HEIGHT ; i++)
            {
                for (int j=0; j<WIDTH ; j++)
                {
                    if( j==0 || j==WIDTH-1 ) cout << GREEN << "#" << RESET;
                    else 
                    {
                        if( i==players[0].Cord_y &&  j==players[0].Cord_x  ) 
                        {
                            cout << RED << "@" << RESET;
                        }
                        else 
                        {
                            if( i==fruit.Cord_y &&  j==fruit.Cord_x  ) cout << YELLOW << "*" << RESET;
                            else 
                            {
                                bool print =false;
                                for (int k = 0; k < players[0].tail.size() ; k++)
                                {
                                    if ( players[0].tail[k].x == j && players[0].tail[k].y == i && !print)
                                    {
                                        cout << RED << "o" << RESET;
                                        print = true;
                                    }
                                }
                                if(!print) cout << " ";
                            }
                        }
                    }
                }
                cout << endl;
            }
            
            //bottom corner
            for (int i=0; i<WIDTH ; i++)
            {
                cout << GREEN << "#";
            }cout << endl;

            cout << "Score game: " << players[0].PlayerScore << RESET << endl;
        };

        void Input()
        {
            char c;
            if( kbhit() )
            {
                c = getchar();
                c = toupper(c);
                snakesDirection prev_dir = players[0].S_Dir;

                switch (c)
                {
                    case 'W':
                        if( prev_dir != DOWN )
                            players[0].S_Dir = UP;
                        break;
                    case 'S':
                        if( prev_dir != UP )
                            players[0].S_Dir = DOWN;
                        break;
                    case 'A':
                        if( prev_dir != RIGHT )
                            players[0].S_Dir = LEFT;
                        break;
                    case 'D':
                        if( prev_dir != LEFT )
                            players[0].S_Dir = RIGHT;
                        break;
                    case 'X':
                        GameOver = true;
                        break;
                    default:
                        break;
                }
            }
        };

        void Logic()
        {
            Body coordenate;
            coordenate.x = players[0].Cord_x;
            coordenate.y = players[0].Cord_y;

            if( players[0].tail.size() > 0 )
            {
                players[0].tail.insert( players[0].tail.begin(), coordenate );
                players[0].tail.pop_back();
            }   

            switch ( players[0].S_Dir )
            {
                case UP:
                    players[0].Cord_y--;
                    break;
                case DOWN:
                    players[0].Cord_y++;
                    break;
                case LEFT:
                    players[0].Cord_x--;
                    break;
                case RIGHT:
                    players[0].Cord_x++;
                    break;
                default:
                    break;
            }
 
            
            if(   players[0].Cord_x <= 0 || players[0].Cord_x >= WIDTH-1   || 
                  players[0].Cord_y < 0  || players[0].Cord_y >= HEIGHT       )
            {
                GameOver = true;
            }

            for (int i = 0; i < players[0].tail.size(); i++)
            {
                if( players[0].Cord_x == players[0].tail[i].x && players[0].Cord_y == players[0].tail[i].y )
                {
                    GameOver = true;
                }
            }
            
            if( players[0].Cord_x == fruit.Cord_x && players[0].Cord_y == fruit.Cord_y  )
            {
                players[0].PlayerScore += 10;
                coordenate.x = fruit.Cord_x;
                coordenate.y = fruit.Cord_y;
                players[0].tail.push_back( coordenate );
                Set_Fruit();
            }
            

        };

};

int main ()
{
    Snake_Game Game;
    Player player1;
    cout << "\nDigite seu nome: ";
    cin >> player1.Name;

    Game.Setup_Player(player1);
    Game.Setup();

    while ( !GameOver )
    {
        Game.Draw();
        Game.Input();
        Game.Logic();
        usleep(100000);
    }
    
    return 0;
}