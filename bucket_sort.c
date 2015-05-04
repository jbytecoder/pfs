#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bucket_sort(int *tab,int n) {
  int max = 0;
  int i,ti;
  
  for(i=0;i<n;++i){
    if(tab[i] > max) max = tab[i];
  }
  
  struct E {
    int data;
    struct E *next;
  };
  
  struct E **tmp = (struct E**)malloc(sizeof(struct E*)*n);
  
  #pragma omp parallel for private(ti,i) schedule(static)	
  for(i=0;i<n;++i) 
  {
      ti = (int) tab[i]*((float)n/(max+1));
      if(tmp[ti]==0){
	tmp[ti] = (struct E*) malloc(sizeof(struct E));
	tmp[ti]->data = tab[i];
	tmp[ti]->next = 0;
      } else {
	 struct E *ne = (struct E*) malloc(sizeof(struct E));
	 ne->data = tab[i];
	 ne->next = tmp[ti];
	 
	 if(ne->data < tmp[ti]->data){
	  tmp[ti] = ne;
	 } else {
	    #pragma omp critical
	   {
	   struct E *p = tmp[ti];
	   while( p->next != 0 && ne->data > p->next->data ){
	      p = p->next;
	   }
	   ne->next = p->next;
	   p->next = ne;
	   }
	 }
	
      }
  }

  
  if(0){
    for(i=0;i<n;++i) 
    {
      if(tmp[i]!=0) {
	struct E *p = tmp[i];
	printf("%d:",i);
	while(p){
	  printf(" %d",p->data);
	  p = p->next;
	}
      }
      printf("\n");
    }
  }
  
  ti=0;
  
  for(i=0;i<n;++i) {
    if(tmp[i]!=0){
      struct E *p = tmp[i],*oldp;
      while(p){
	tab[ti++] = p->data;
	oldp = p;
	p = p->next;
	free(oldp);
      }
    }
  }
  
  free(tmp);
}

int main(int argc,char** argv){
  srand(time(NULL));
  
  int n = atoi(argv[1]),i;
  omp_set_num_threads(atoi(argv[2]));
  int *tab = (int*) malloc(sizeof(int)*n);
  
  // printf("input array: \n");
  for(i=0;i<n;++i){
    tab[i] = rand()%1000;
  //  printf(" %d",tab[i]);
  }
  //printf("\n");
  unsigned long long t = time(NULL);
  bucket_sort(tab,n);
  printf("%d\n",time(NULL)-t);
  //printf("output array: \n");
  //for(i=0;i<n;++i){
  //  printf(" %d",tab[i]);
  // }
  // printf("\n");
	return 0;  
}
