#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"
#define max 100000

int cc;
int ccnum[max];

#include "un_directed_graph.h"
#define max 100000//Max of vertex

Graph createGraph();
void addVertex(Graph graph,char *word,int id);
char *getVertex(Graph graph, int id);
void addEdge(Graph graph, int v1, int v2);
void add_2_Edge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);

int degree(Graph graph, int v, int* output);
void dropGraph(Graph graph);
void delSpace(char *string);
int compare_A(char *v1, char *v2);
int getID(Graph graph);
void printTPLT(Graph graph, JRB TPLT);

int main(int argc,char *argv[])
{
  int ID,start,stop;
  Graph graph = createGraph();
  char word[10];
  int output[max],path[1000],length;
  FILE * file = fopen("data.txt","r");
  JRB u,v,TPLT = make_jrb();
  if (file == NULL)
    {
      printf("ERROR INPUT\n");return 0;
    }
  ID = 1;
  while (fscanf(file,"%s",word) > 0)
    {
      delSpace(word);
      if (strlen(word) != 5) {printf("Loi data\n");return 0;};
      addVertex(graph,word,ID);
      //printf("%s %d\n",word,ID);
      ID++;
    }
  fclose(file);
  printf("Wait a minute!\n");
  jrb_traverse(v,graph.vertices)
    {
      jrb_traverse(u,graph.vertices)
	{
	  if ((u != v) && (compare_A(jval_s(u->key),jval_s(v->key)) == 1))
	    {
	      // printf("%s %s\n",jval_s(u->key),jval_s(v->key));
	      add_2_Edge(graph,jval_i(u->val),jval_i(v->val));
	    }
	}
    }//end traverse out
  DFS(graph,TPLT);
  printf("So thanh phan lien thong la: %d\n",cc);
  
  //in TPLT ? co the cai dat chon TPLT thu may
  printf("Co in thanh phan lien thong khong?(y/n)\n");
  fgets(word,10,stdin);delSpace(word);
  if (strcmp(word,"y") == 0) printTPLT(graph,TPLT);
  
  printf("Nhap tu xuat phat: ");
  start = getID(graph);
  //Nhap tu ket thuc
  printf("Nhap tu ket thuc: ");
  stop = getID(graph);
  ID = sortBFS(graph,start,stop,path,&length);
  if (ID == 0) printf("Khong co duong di\n");
  else (printf("TPLT thu %d\n",ccnum[start]));
  for (int i = 0;i < length;i++)
    {
      strcpy(word,getVertex(graph,path[i]));
      if (i != (length-1))
	printf("%s ==> ",word);
      else printf("%s\n",word);
    }
  
}//end main

int compare_A(char *v1, char *v2)
{
  int count = 0;
  for (int i = 0;i < 5;i++)
    {
      if (v1[i] != v2[i]) count++;
    }
  if (count == 1) return 1;
  else return 0;
}

void delSpace(char *a)
{
  long n = strlen(a);
  if ((a[n] == '\0') && (a[n-1] == '\n'))
    a[n-1] = '\0';
}

int getID(Graph graph)
{
  char word[10];
  JRB u;
  fgets(word,10,stdin);delSpace(word);
  //printf("%s\n",word);
  u = jrb_find_str(graph.vertices,word);
  if (u == NULL) return 0;
 else return jval_i(u->val);
}

 void printTPLT(Graph graph, JRB TPLT)
{
  JRB node,tree,v;
  int n,i = 0;
  jrb_traverse(node,TPLT)
    {
      i++;
      printf("Thanh phan lien thong thu %d\n",i);
      tree = (JRB) jval_v(node->val);
      jrb_traverse(v,tree)
	{
	  n = jval_i(v->key);
	  printf("%s\n",getVertex(graph,n));
	}
    }//end out
}
