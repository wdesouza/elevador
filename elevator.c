/*
   Fun��o: Fornecer intera��o e gerenciar um elevador (maquete),
    sistema de siula��o.
   Plataforma: Linux. Padr�o ISO
   Autor: Welliton de Souza - well309@gmail.com
   Data: 25/09/09
*/
//Bibliot�cas utilizadas
#include <stdio.h> // Fun��es de E/S
#include <stdlib.h> // Func�es basicas
#include <sys/io.h> // Fun��es ioperm, outb
#include <unistd.h> //Fun��o usleep
#include "getch.h" //Fun��o getch() - n�-oficial
//Defini��es
#define LPT 0x378 //Porta paralela
#define STOP 0 //parar
#define PINO2 3 //subir
#define PINO3 1 //descer
#define TIMEUP 2700000  //Tempo de subir (em microsegundos) - 1.000.000ms = 1s
#define TIMEDOWN 2590000 //Tempo de descer (em microsegundos)
//Prot�tipos das fun��es
int keyToInt (int value);
void console(void);
void toMove(int value);
void viewPosition(void);
//Vari�veis globais
int position;
//Fun��o principal
int main ()
{
   if (ioperm (LPT, 3, 1) == 1) //Ativa e verifica a Porta Paralela
   {
      printf ("Erro ao acessar o endere�o 0x378 (Porta Paralela).\n");
      exit (1); //Em caso de erro
   }
   viewPosition(); //Exibe a posi��o atual (no caso T�rreo)
   console(); //Chama o console do elevador
   outb(STOP, LPT); 
   ioperm(LPT, 3, 0); //Desativa a Porta Paralela
   return 0; //Fim do programa
}
//Converte a entrada do teclado para inteiro
int keyToInt (int value)
{
   switch (value)
   {
      case 48: {return 0;} //Tecla 0
      case 49: {return 1;} //Tecla 1
      case 50: {return 2;} //Tecla 2
      case 51: {return 3;} //Tecla 3
		case 27: {return -1;} //Tecla Esc
   }  
}
//Console do elevador
void console(void)
{
   int goTo, in; //Objetivo, entrada
   printf ("Console do Elevador.\n");
   do
   {
      printf (": ");
      in = getch(); //Recebe tecla
      goTo = keyToInt(in); //Recebe inteiro
      if ((goTo >= 0) && (goTo <= 3) && (goTo != position)) //Verifica��o
         toMove(goTo); //Chama do procedimento (destino)
      else
         if (goTo == -1) //Caso a mesma posi��o
         {
            toMove(0); //Desce o elevador ate o t�rreo
            printf ("Sair.\n");
            goTo = -1;
         }
         else //Qualquer outro valor
            printf ("Erro.\n");
   }
   while (goTo != -1); //Condi��o do loop 
}
//Procedimento que gerencia o elevador emitindo sinais na Porta Paralela
void toMove (int value)
{
   if (value > position) //Para subir
   {
      printf ("Subindo.\n");
      while (value > position) //Enquanto nao chegar ao destino
      {
			outb(PINO3, LPT); //para subir
         usleep(TIMEUP); //Tempo em segundos
         position++; //Incrementa 1 � posi��o atual
         viewPosition(); //Chama o procedimento para exibir a posi��o
      }
   }
   else if (value < position) //Para descer
   {
      printf("Descendo.\n");
      while (value < position)
      {
			outb(PINO2, LPT); //para descer, liga pino 2 e apaga pino 3
         usleep(TIMEDOWN);
         position--; //Decrementa 1 � posi��o atual
         viewPosition();
      }
   }
} 
//Exibe a posi��o atual no console e no painel
void viewPosition (void)
{
   if (position == 0) //Caso t�rreo
      printf ("\nPosi��o: T�rreo.\n");
   else //Outros andares
      printf ("\nPosi��o: %d.o andar.\n", position);
	outb(STOP, LPT); //p�ra o elevador
}     
