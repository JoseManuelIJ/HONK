#include <stdio.h>
#include <stdlib.h>
/*
    funcion para encontrar el valor de la convolucion.
*/
int valor(int matriz[10][10],int i,int j) 
{
    int sum;
    int MC[3][3]= {{0,1,0},
        {1,-4,1},
        {0,1,0}    
        };
    sum=0;
    for(int f=0;f<3;f++)
    {
        for(int c=0;c<3;c++)
        {
            sum=sum+matriz[i+f-1][j+c-1] * MC[f][c];
        }
    }
    return sum;
}
int main () {
    FILE *fp, *fp2;
   int matriz [10][10],fil,col;
   int newM[sizeof(matriz)][sizeof(matriz[0])];
  
   fp = fopen ("prueba.txt", "w");
   //creando matriz de prueba
   for(int i = 0;i<10;i++)
   {
       for(int j=0;j<10;j++)
       {
           matriz[i][j]=1 +rand() % 11;
           fprintf(fp, "%d %c", matriz[i][j],',');
       }
       fprintf(fp, "\n");
   }   
   fclose(fp);
   //exit(1);
   fp2 = fopen ("respuesta.txt", "w");
   //convolucionando la matriz
   col=sizeof(matriz[0])/4;
   fil=sizeof(matriz)/sizeof(matriz[0]);
   for(int i=0;i<fil;i++)
   {
       for(int j=0;j<col;j++)
       {
           if (i<1 || j<1 || (i>fil-2) || (j>col-2))
           {
               newM[i][j]=0;
           }
           else
           {
               newM[i][j]=valor(matriz,i,j);
           }
           fprintf(fp2, "%d %c", newM[i][j],',');
       }
       fprintf(fp2, "\n");
   }
   fclose(fp2);
   return(0);
}