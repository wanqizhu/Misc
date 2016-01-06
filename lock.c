#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#define MAXV 10000 // If graph has more vertices than this, must set manually.
#define TRUE 1
#define FALSE 0
#define MAXINT 10000000


typedef struct edgenode {
  int y; //label of the node
  int weight;
  struct edgenode *next;
} edgenode;

// adjacency list format
typedef struct {
  edgenode *edges[MAXV+1];
  //int degree[MAXV+1];
  int color[MAXV+1];
  //int stime[MAXV+1];
  //int ftime[MAXV+1];
  int nvertices;
  int nedges;
  //int directed;
  int maxv; //largest label of vertex
} graph;


// linked list
typedef struct Node {
  int data;
  struct Node* next;
  struct Node* prev;
} node;

typedef struct {
  int size;
  node *start;
  node *end;
} llist;


int N, CYCLE, TIME;
graph *G;
llist *vtx_order;


llist *list_init(){
  llist *list = malloc(sizeof(llist));
  list->start    = NULL;  
  list->end   = NULL; 
  list->size    = 0;       // number of elements currently
  return list;
}



// To Enqueue an integer
void enqueue(llist *q, int x) {
  struct Node* temp = malloc(sizeof(struct Node));
  temp->data = x; 
  temp->next = NULL;
  if (q->start == NULL) {
    q->start = temp;
  } else {
    q->end->next = temp;
    
  }
  temp->prev = q->end;
  q->end = temp;
  q->size++;
}

/*
// To Dequeue an integer and return it
Node *Dequeue(llist *q) {
  struct Node* temp = q->start;
  if(q->start == NULL) {
    printf("Queue is Empty\n");
    return NULL;
  }
  if(q->start == q->end) {
    q->start = q->end = NULL;
  }
  else {
    q->start = q->start->next;
  }

  q->size--;
  return temp;
}*/







graph *create_graph(/*int directed*/) {
  graph *G = malloc(sizeof(graph));
  int i;
  for (i=0; i<=MAXV; i++) {
    G->edges[i] = NULL;
    //G->degree[i] = 0;
    G->color[i] = 0;
    //G->stime[i] = G->ftime[i] = INT_MAX;
  }
  G->nvertices = G->nedges = G->maxv = 0;
  //G->directed = directed;
  return G;
}

int insert_edge(graph *G, int u, int v, int weight) {

  if (v>MAXV) return -4;

  edgenode *new_edge = malloc(sizeof(edgenode));
  new_edge->y = v;
  new_edge->weight = weight;

  if (G->edges[u] == NULL) {
    new_edge->next = NULL;
    G->nvertices++;
  } else new_edge->next = G->edges[u];

  G->edges[u] = new_edge;

  G->nedges++;
  //G->degree[u]++;

  if (v>G->maxv) G->maxv = v;
  if (u>G->maxv) G->maxv = u;

  return 0;
}


/*
void print_graph(graph *G) {
  for (int u=0; u<=G->maxv; u++) {
    edgenode *v = G->edges[u];
    while (v != NULL) {
      printf("%d -> %d: %d\n", u, v->y, v->weight);
      v = v->next;
    }
    if (G->degree[u] !=0) printf("\n");
  }

 // for (int i=0; i<=G->maxv; i++)
    //printf("Degree of %d: %d\n", i, G->degree[i]);
  //printf("maxv: %d, nvertices: %d, nedges: %d\n", G->maxv, G->nvertices, G->nedges);
}
*/


void DFS(graph *G, int n) {
  //TIME++;
  G->color[n] = 1;
  //G->stime[n] = TIME;
  edgenode *t = G->edges[n];
  int v;
  while (t != NULL) {
    v = t->y;
    if (G->color[v] == 0) {
      DFS(G, v);
    } else if (G->color[v] == 1) {
      CYCLE = 1;
      return;
    }

    t = t->next;
  }
  enqueue(vtx_order, n);
  G->color[n] = 2;
  //G->ftime[n] = TIME;
}






int main() {
  G = create_graph(/*1*/);
  FILE *fin = fopen("lock.in", "r");
  FILE *fout = fopen("lock.out", "w");

  /*if (fin==NULL){
    printf("Error opening file.\n");
    exit(-1);
  }*/
  int i, c;
  char *line = malloc(MAXV*sizeof(char));
  
  fgets(line, MAXV, fin);
  N = (int)strtol(line, &line, 10);


  // scans in the N pairs of lines
  for (i=2; i<2*N+2; i++) {
    fgets(line, MAXV, fin);
    while (line[0] != '\n' && line[0] != 'N' && line[0] != '\0') {
      c = (int)strtol(line, &line, 10);
      //printf("%d ", c);

      if (i%2) c = insert_edge(G, i/2, c, 1);
      else c = insert_edge(G, c, i/2, 1);

      assert (c == 0);

    }
    //printf("\n");
  }

  for (i=1; i<=N; i++) {
    insert_edge(G, 0, i, 1); // make an edge from vtx 0 to everything
  }

  //print_graph(G);
  vtx_order = list_init();
  DFS(G, 0); // DFS from vtx 0

  // impossible
  if (CYCLE) {
    fprintf(fout, "IMPOSSIBLE\n");

  } else{
    node *temp = vtx_order->end->prev; // skip over vtx 0
    while (temp != NULL) {
      fprintf(fout, "%d\n", temp->data);
      temp = temp->prev;
    }
  }


  fclose(fin);
  fclose(fout);
  return 0;
}



