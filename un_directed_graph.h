#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "jrb.h"
#include "dllist.h"

typedef struct {
   JRB edges;
   JRB vertices;
} Graph;

Graph createGraph();
void addVertex(Graph graph, char *word,int id);
char *getVertex(Graph graph,int id);
void addEdge(Graph graph, int v1, int v2);
void add_2_Edge(Graph graph, int v1, int v2);
int hasEdge(Graph graph, int v1, int v2);

int degree(Graph graph, int v, int* output);
void dropGraph(Graph graph);
void explore(Graph graph,int vertex,int *visited, JRB TPLT);
int DFS(Graph graph,JRB TPTL);
int sortBFS(Graph graph,int start,int stop,int *path,int *length);
void addTPLT(JRB TPLT,int cc,int id);
int hasTPLT(JRB TPLT,int cc, int id);
  
Graph createGraph()
{
   Graph g; 
   g.edges = make_jrb();  
   g.vertices = make_jrb();  
   return g;
}

void addVertex(Graph g,char *word,int id)
{
     JRB node = jrb_find_str(g.vertices,word);
     if (node==NULL) // only add new vertex 
       jrb_insert_str(g.vertices,strdup(word),new_jval_i(id));            
}

char *getVertex(Graph g, int id)
{
  JRB node;
  jrb_traverse(node,g.vertices)
    {
      if (id == jval_i(node->val))
	return jval_s(node->key);
    }
}     

void addEdge(Graph graph, int v1, int v2)
{
     JRB node, tree;
     if (!hasEdge(graph, v1, v2))
     {
        node = jrb_find_int(graph.edges, v1);
        if (node==NULL) {
           tree = make_jrb();
           jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        } else
        {
           tree = (JRB) jval_v(node->val);   
        }
        jrb_insert_int(tree, v2, new_jval_i(1));
     }//V1->V2
}

void add_2_Edge(Graph graph, int v1, int v2)
{
  addEdge(graph,v1,v2);
  addEdge(graph,v2,v1);
}

int hasEdge(Graph graph, int v1, int v2)
{
    JRB node, tree;
    node = jrb_find_int(graph.edges, v1);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    if (jrb_find_int(tree, v2)==NULL)
       return 0;
    else
       return 1;       
}

int degree (Graph graph, int v, int* output)
{
    JRB tree, node;
    int total;
    node = jrb_find_int(graph.edges, v);
    if (node==NULL)
       return 0;
    tree = (JRB) jval_v(node->val);
    total = 0;   
    jrb_traverse(node, tree)
    {
       output[total] = jval_i(node->key);
       total++;                
    }
    return total;   
}

void dropGraph(Graph graph)
{
    JRB node, tree;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB) jval_v(node->val);
        jrb_free_tree(tree);
    }
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
}

int sortBFS(Graph graph, int start, int stop,int *path,int *length)
{
  int tmp[max],prev[max],visited[max] = {};
  int output[max],u,v,number;
  Dllist node, queue = new_dllist();
   
   visited[start] = 1;
   prev[start] = start;
   dll_append(queue,new_jval_i(start));
   while (dll_empty(queue) == 0)
     {
       node = dll_first(queue);
       u = jval_i(node->val);
       //printf("%d\n",u);
       dll_delete_node(node);
       if (u == stop) break;
       number = degree(graph,u,output);
       for (int i = 0;i < number;i++)
	 {
	   v = output[i];
	   if (visited[v] == 0)
	     {
	       prev[v] = u;
	       visited[v] = 1;
	       dll_append(queue,new_jval_i(v));
	     }
	 }//end for
     }//end while
   if (u == stop)
     {
       tmp[0] = stop;
       number = 1;
       while (stop != start)
	 {
	   //printf("%d\n",stop);
	   stop = prev[stop];
	   tmp[number++] = stop;
	 }
       for (int i = number - 1;i >= 0;i--)
	 path[number-1-i] = tmp[i];
       *length = number;
       return 1;
     }
   else return 0;
}

void explore(Graph graph,int vertex,int *visited,JRB TPLT)
{
  int output[max],n,v;
  Dllist node,stack = new_dllist();
  visited[vertex] = 1;
  dll_prepend(stack,new_jval_i(vertex));
  
  while( !dll_empty(stack) )
    {
      node = dll_first(stack);
      v = jval_i(node->val);
      dll_delete_node(node);
      ccnum[v] = cc;
      addTPLT(TPLT,cc,v);
      n = degree(graph,v,output);
      for (int i = 0;i < n;i++)
	{
	  v = output[i];
	  if (visited[v] == 0)
	    {
	      dll_prepend(stack,new_jval_i(v));
	      visited[v] = 1;
	    }
	}
    }//end while
}

int DFS(Graph graph,JRB TPLT)
{
  JRB node;
  int v,visited[max] = {};
  cc = 0;
  jrb_traverse(node,graph.vertices)
    {
      v = jval_i(node->val); 
      if (visited[v] == 0)
	{
	  // printf("%d %s\n",v,jval_s(node->key));
	  cc++;
	  explore(graph,v,visited,TPLT);
	}
    }//end traverse
}

int hasTPLT(JRB TPLT,int cc, int id)
{
  JRB tree, node;
  node = jrb_find_int(TPLT,cc);
  if (node == NULL) return 0;
  tree = (JRB) jval_v(node->val);
  node = jrb_find_int(tree,id);
  if (node == NULL) return 0;
  else return 1;
}

void addTPLT(JRB TPLT,int cc,int id)
{
  JRB node, tree;
  if (!hasTPLT(TPLT, cc, id))
     {
       node = jrb_find_int(TPLT, cc);
       if (node==NULL) {
	 tree = make_jrb();
	 jrb_insert_int(TPLT,cc, new_jval_v(tree));
       } else
	 {
           tree = (JRB) jval_v(node->val);   
	 }
       jrb_insert_int(tree,id, new_jval_i(1));
     }//V1->V2
}
