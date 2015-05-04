#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

void insert_sort(int *tab,int n) {
  int i;
  for(i=1;i<n;++i) {
    int j=i-1;
    int e = tab[i];
    while(j>=0&&e<tab[j]){
      tab[j+1]=tab[j];
      --j;
    }
    tab[j+1] = e;
  }
}
 

void bucket_sort(int *tab, int n,int p)
{   
   int **tmp = malloc(sizeof(int*)*(p));
   int *s = malloc(sizeof(int)*(p));
   
   int i=0;
   for(i=0;i<p;++i) {
      tmp[i] = (int*) malloc(sizeof(int)*(n));
      s[i] = 0;	
   }
   		
   int max = 0;
   for(i=0;i<n;++i) {
	if( tab[i] > max ) {
		max = tab[i];
	}
   }
   int bi = 0;
   
  
   for(i=0;i<n;++i) {
       bi = (int) tab[i]*((float)p/(max+1));
       tmp[bi][(s[bi]++)] = tab[i]; 
   }
   
    #pragma omp parallel for private(i) schedule(static)
    for(i=0;i<p;++i) {

   /*   printf("bucket %d: ",i);
      for(j=0;j<tmp[i][0];++j){
	printf(" %d",tmp[i][j+1]);
      }
      printf("\n");*/	
 
    insert_sort(tmp[i],s[i]);
	
     /* printf("bucket %d: ",i);
      for(j=0;j<tmp[i][0];++j){
	printf(" %d",tmp[i][j+1]);
      }
      printf("\n");*/	
   }
   
   int ti=0;
   for(i=0;i<p;++i){
       int j = 0;
       while(j<s[i]){
	tab[ti++]=tmp[i][j];
        ++j;
       }
      // free(tmp[i]);
   }
  // free(tmp);

}

int main(int argc,char** argv){
  srand(time(NULL));
  
  int n = atoi(argv[1]),i;
  int *tab = (int*) malloc(sizeof(int)*n);
  omp_set_num_threads(atoi(argv[2]));
  
  //printf("input array: \n");
  for(i=0;i<n;++i){
    tab[i] = rand()%1000;
 //   printf(" %d",tab[i]);
  }
 // printf("\n");
  unsigned long long t = time(NULL);	
  bucket_sort(tab,n,atoi(argv[3]));
  printf("%lld\n",time(NULL)-t);
 // printf("output array: \n");
 // for(i=0;i<n;++i){
 //   printf(" %d",tab[i]);
 //  }
 // printf("\n");
 return 0;  
}
