#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "jrb.h"
#include "dllist.h"
#define max 100000

int cc;
int ccnum[max];
int clock;
int prev[max] = {};
int post[max] = {};

#include "directed_graph.h"
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
int compare_B(char *v1, char *v2);
int getID(Graph graph);
void printTPLT(Graph graph, JRB TPLT,int cc);

int main(int argc,char *argv[])
{
  int visited[max] = {};
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
  ID = 0;
  jrb_traverse(v,graph.vertices)
    {
      jrb_traverse(u,graph.vertices)
	{
	  if ((u != v) && (compare_B(jval_s(u->key),jval_s(v->key)) == 1))
	    {
	      // printf("%s %s\n",jval_s(u->key),jval_s(v->key));
	      addEdge(graph,jval_i(v->val),jval_i(u->val));
	      ID++;
	    }
	}
    }//end traverse out
  DFS(graph,TPLT);
  timTPLT(graph,TPLT);
  //a)
  
  printf("a) So thanh phan lien thong manh: %d\n",cc);
  //b)
  
  printf("b) Nhap 1 tu u va in ra cac TPLT manh voi u: ");
  fgets(word,10,stdin);delSpace(word);
  v = jrb_find_str(graph.vertices,word);
  if ( v == NULL) return 1;
  start = jval_i(v->val);
  
  printTPLT(graph,TPLT,ccnum[start]);
  //c)
  printf("c) Tim duong di ngan nhat\n");
  printf("Nhap tu xuat phat: ");
  start = getID(graph);
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
  
}

int compare_B(char *v1, char *v2)
{
  int count = 0, n ;
  int m1[26] = {},m2[26] = {};
 //v2 abcde
  for (int i = 0;i < 5;i++)
    {
      n = v2[i] - 'a';
      m2[n]++;//tru di o m1 neu v
    }
  //v1 a/bcde
  for (int i = 1;i < 5;i++)
    {
      n = v1[i] - 'a';
      m1[n]++;
    }
  count = 0;
  for (int i = 1;i < 5;i++)
    {
      n = v1[i] - 'a';
      if (m2[n] >= m1[n]) count++;
    }
  if (count == 4) return 1;
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

void printTPLT(Graph graph, JRB TPLT,int cc)
{
  JRB node,tree,v;
  int n,i = 0;
  node = jrb_find_int(TPLT,cc);
  if (node == NULL) return;
  tree = (JRB) jval_v(node->val);
  printf("Thanh phan lien thong thu %d\n",cc);
  jrb_traverse(v,tree)
    {
      n = jval_i(v->key);
      printf("%s\n",getVertex(graph,n));
    }
}
