/*
   Função getch() (semelhante ao getch do ambiente Windows).
*/
#include <termios.h> // Para funcao getch()
#include <unistd.h> //para funcao getch()
//Protótipo da função
int getch ( );
//Função que lê uma tecla e retorna o respectivo valor em inteiro
int getch( )
{
  struct termios oldt,
                 newt;
  int            ch;
  tcgetattr( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  return ch;
}
