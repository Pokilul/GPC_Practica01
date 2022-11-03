/*
Graficos por Computadora

Nombre: Práctica 01
Nombre del archivo: pra1.c
Fecha:  04-11-2022
Programador: Morales Flores Gael Alexis

Objetivo: Implementar la animación de un reloj
          analógico, que muestre las tres manecillas 
          correspondientes a las horas, minutos y segundos.

Compilar:
gcc -c pra1.c
Enlazar:
gcc pra1.o -o run  -l glut32 -l opengl32
*/

#include<gl\glut.h>
#include<math.h>
#include<stdio.h>
#include <windows.h>

typedef enum{
  NEGRO, AZUL, VERDE, CYAN, 
  ROJO, MAGENTA, AMARILLO, 
  BLANCO, GRIS, GRIS_OSCURO, 
  GRIS_CLARO
}COLOR;

float PALETA_COLORES[11][3]={ 
  0, 0, 0, //NEGRO
  0, 0, 1, //AZUL
  0, 1, 0, //VERDE
  0, 1, 1, //CYAN
  1, 0, 0, //ROJO
  1, 0, 1, //MAGENTA
  1, 1, 0, //AMARILLO
  1, 1, 1, //BLANCO
  0.5,0.5,0.5, //GRIS
  0.25,0.25,0.25, //GRIS_OSC
  0.8,0.8,0.8 //GRIS_CLARO
}; 

// Definición de estructuras
typedef struct{ 
  float x;
  float y;
}PUNTO;

typedef struct{ 
  PUNTO ini;
  PUNTO fin;
  COLOR color;
}LINEA; 

typedef struct{ 
  PUNTO c;
  float r;
}CIRCULO;

// Prototipos de función
void DibujarTexto(char *Texto, float x, float y);
void AsignaColor(COLOR c);
void display(void);
void Circulo(CIRCULO ci);
void Linea(LINEA L);
void RotacionLinea(LINEA *L,float xr, float yr, float Theta);
void animar(void);

// Variables globales
unsigned int minFlag = 0, hourFlag = 0;
const float pi = 3.14159;
LINEA segundero, minutero, horario;

int main(int argc, char **argv){
  // Inicialización de manecillas del reloj
  segundero.ini.x = 0;
  segundero.ini.y = 0;
  segundero.fin.x = 0;
  segundero.fin.y = 6;
  segundero.color = BLANCO;

  minutero.ini.x = 0;
  minutero.ini.y = 0;
  minutero.fin.x = 0;
  minutero.fin.y = 5.5;
  minutero.color = AZUL;

  horario.ini.x = 0;
  horario.ini.y = 0;
  horario.fin.x = 0;
  horario.fin.y = 5;
  horario.color = VERDE;

  glutInit(&argc,argv);
  glutInitWindowSize(600,600);
  glutCreateWindow("Practica 1: animacion de reloj");
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-10.0,10.0,-10.0,10.0,1.0,-1.0); 
  glMatrixMode(GL_MODELVIEW);

  glutDisplayFunc(display);

  glutIdleFunc(animar);

  glutMainLoop();
  return 0;
}

void display(void){
  float ang = 30, theta, x, y;
  char hora[5];
  int k;
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
    
  AsignaColor(BLANCO);
  CIRCULO C1;
  C1.c.x = 0;
  C1.c.y = 0;
  C1.r = 10;
  Circulo(C1);

  LINEA L1;
  L1.color = BLANCO;
  L1.ini.x = 8;
  L1.ini.y = 0;
  L1.fin.x = 10;
  L1.fin.y = 0;

  //DIBUJA LOS SEGMENTOS DE LAS HORAS DE LA CARÁTULA
  for (k=0; k<12; k++){
    RotacionLinea(&L1, 0, 0, ang*(pi/180.0));
    Linea(L1);
  }
  //DIBUJA LOS NÚMEROS CORRESPONDIENTES A LAS HORAS
  k=1;  
  for(theta=1.04719; theta>-5; theta-=0.523598){
    x = 0 + 7*cos(theta);
    y = 0 + 7*sin(theta);
    sprintf(hora, "%i", k);
    DibujarTexto(hora, x, y);
    k++;
  }

  //DIBUJA EL SEGUNDERO, MINUTERO Y HORA
  Linea(segundero);
  Linea(minutero);
  Linea(horario);  

  glFlush();  
}

void animar(void){
    RotacionLinea(&segundero, 0, 0, -0.104719);
    if(minFlag == 60){
      RotacionLinea(&minutero, 0, 0, -0.104719);    
      minFlag=0;
    }
    if(hourFlag == 3600){
      RotacionLinea(&horario, 0, 0, -(2*pi)/12);    
      hourFlag=0;
    }   
    sleep(1);
    //usleep(1);
    minFlag++;
    hourFlag++;
    glutPostRedisplay();
}

void RotacionLinea(LINEA *L,float xr, float yr, float Theta){
  float xp,yp;
  xp = xr + (L->ini.x-xr) * cos(Theta) - (L->ini.y-yr) * sin(Theta);
  yp = yr + (L->ini.x-xr) * sin(Theta) + (L->ini.y-yr) * cos(Theta);
  L->ini.x = xp;
  L->ini.y = yp;
  xp = xr + (L->fin.x-xr) * cos(Theta) - (L->fin.y-yr) * sin(Theta);
  yp = yr + (L->fin.x-xr) * sin(Theta) + (L->fin.y-yr) * cos(Theta);
  L->fin.x = xp;
  L->fin.y = yp;
}

void DibujarTexto(char *Texto, float x, float y){
  int k = 0;
  glRasterPos2f(x,y);
  while(Texto[k]!='\0'){
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,Texto[k]);
    k++;
  }
}

void AsignaColor(COLOR c){
  glColor3f(PALETA_COLORES[c][0],PALETA_COLORES[c][1],PALETA_COLORES[c][2]);
}

void Circulo(CIRCULO ci){
  float x,y,theta;
  glBegin(GL_LINE_LOOP);
  for(theta=0; theta<6.28; theta+=0.1){
    x=ci.c.x+ci.r*cos(theta);
    y=ci.c.y+ci.r*sin(theta);
    glVertex2f(x,y);
  }
  glEnd(); 
}

void Linea(LINEA L){
  AsignaColor(L.color);
  glBegin(GL_LINES);
  glVertex2f(L.ini.x,L.ini.y);
  glVertex2f(L.fin.x,L.fin.y);
  glEnd();
}