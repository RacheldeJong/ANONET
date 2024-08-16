#include "nauty_graph.h"
#include "twinnode.h"

// Read number of nodes from file
int read_n(const char* file_name){
    FILE* file;
    char c;
    int n = 0;

    if((file = fopen(file_name, "r")) == NULL){ 
        printf("Error: File %s could not be opened\n", file_name);
        return 0;
     }
     // Read n
    while (c != EOF){
        if(c == '\n' && (c=getc(file)) != '!') break;
        if(c == 'n' && (c=getc(file)) == '='){
           c = getc(file);
           while (ISDIGIT(c)){
              n *= 10;
              n += c - '0';
              c = getc(file);
           }
         }
         else c = getc(file);
    }
    return n;
}

const int Graph::get_number_nodes(){
   return graph.nv; 
}

const int Graph::get_number_edges(){
   return graph.nde;
}

const bool Graph::is_directed(){
   return directed;
}

void Graph::add_node(){

}

const bool Graph::get_twin_init(){
   return twin_init;
}

void Graph::init_twin(){
   twinnodes = new Twinnodes(this);
   twinnodes->compute();
   twin_init = true;

}

int Graph::add_edge_dir(const int src, const int trgt){
   // Fid position src node
   int pos_src = graph.v[src];
   int deg_src = graph.d[src];
   int pos_next;

   // Get index for first out of bounds element
   if(src < graph.nv -1){
      pos_next = graph.v[src + 1];
   }
   else{
      pos_next = graph.nde;
   }

   // Check if there is still room
   if(pos_src + deg_src >= pos_next && src != graph.nv -1){
      std::cout<<"Error: not enough space left for node"<<std::endl;
      return -1;
   }

   // Add edge, update degree src
   graph.e[pos_src + deg_src] = trgt;
   graph.d[src] += 1;

   updated_edges.push_back(std::make_pair(std::make_pair(src, trgt), OP_ADD));

   return 0;
}

int Graph::add_edge(const int src, const int trgt){
   if(add_edge_dir(src, trgt) == -1) return -1;
   else if(add_edge_dir(trgt, src) == -1){
      delete_edge(src, trgt);
      return -1;
   }
   updated_edges.push_back(std::make_pair(std::make_pair(src, trgt), OP_ADD));
   updated_edges.push_back(std::make_pair(std::make_pair(trgt, src), OP_ADD));

   graph.nde += 2;
   return 0;
}

void Graph::delete_node(){
   // TODO
}

void Graph::delete_edge_dir(const int src, const int trgt){
   int pos_trgt = -1;
   int neighbor;
   int node_it;
   bool found = true;

   for(node_it = 0; node_it < graph.d[src]; ++node_it){
        neighbor = graph.e[graph.v[src] + node_it];
        
        // Find pos trgt, update degree
        if(neighbor == trgt){
         pos_trgt = graph.v[src] + node_it;
         graph.e[pos_trgt] = -1;
         break;
        }
    }

   if(pos_trgt >= 0){
      // Move edges of node one to the left
      for(node_it = pos_trgt; node_it < graph.v[src] + graph.d[src]; ++node_it){
         graph.e[node_it] = graph.e[node_it + 1];
      }
      graph.d[src] -= 1;

   }
   updated_edges.push_back(std::make_pair(std::make_pair(src, trgt), OP_DELETE));
   graph.nde -= 1;
}

void Graph::delete_edge(const int src, const int trgt){
   
   // TODO: allow degree 0 nodes?
   if(graph.d[src] == 1 || graph.d[trgt] == 1){
      //std::cout<< "Warning: node without edge created"<<std::endl;
      //return;
   }
   delete_edge_dir(src, trgt);
   delete_edge_dir(trgt, src);
   updated_edges.push_back(std::make_pair(std::make_pair(src, trgt), OP_DELETE));
   updated_edges.push_back(std::make_pair(std::make_pair(trgt, src), OP_DELETE));
}

std::vector< std::pair< std::pair<int, int>, int> > Graph::get_updated_edges(){
   return updated_edges;
}


std::set<int> Graph::get_nodes(){
   return nodes;
}

std::vector< std::pair<int, int> > Graph::get_edges(){
   std::vector< std::pair<int, int> > edges;
   std::set<int> neighbors;

   for(auto node : nodes){
      neighbors = get_neighbors(node);
      for(auto nbh : neighbors){
         if(!directed && node > nbh) continue;
         edges.push_back(std::make_pair(node, nbh));
      }
   }
   return edges;
}

const std::set<int> Graph::get_neighbors(const int v){
    std::set<int> neighbors;
    int neighbor;
    for(int node_it = 0; node_it < graph.d[v]; ++node_it){
        neighbor = graph.e[graph.v[v] + node_it];
        neighbors.insert(neighbor);
    }
    return neighbors;
}

const int Graph::get_degree(const int v){
    return graph.d[v];
}

const std::unordered_set<int> Graph::get_neighborhood_nodes(const int v, const int d, int &edges){
   std::queue<int> q; // Queue with nodes to be visited
   std::unordered_set<int> vis; //Visited nodes (unordered_set for quick access when in set)
   
   int cur_node;
   int depth = 0;
   int time_to_increase_depth = 1;
   edges = 0;
   vis.clear();
   q.push(v);
   vis.insert(v);
   

   // Iterate over nodes to be visited
   while(!q.empty()){
        
      cur_node = q.front();
      // Keep track of when to increase depth of search
      if((time_to_increase_depth -= 1) == 0){
         if(++depth > d) break;
         time_to_increase_depth = q.size();
      }
      q.pop();

      // Iterate over edges of current node
      for(auto nbr_node : get_neighbors(cur_node)){
         edges++;
         // Do not add to queue again if node is already added
         if(vis.find(nbr_node) != vis.end()) continue;
         q.push(nbr_node);
         vis.insert(nbr_node);
      }
   } 
   return vis;
}

const std::unordered_set<int> Graph::get_neighborhood_nodes_directed(const int v, const int d, int &edges){
   std::queue< int > q; // Queue with nodes to be visited
   std::unordered_set<int> vis; //Visited nodes (unordered_set for quick access when in set)
   
   int cur_node, nbr_node;
   int depth = 0;
   int time_to_increase_depth = 1;
   edges = 0;
   vis.clear();
   q.push(v);
   vis.insert(v);

   // Iterate over nodes to be visited
   while(!q.empty()){
        
      cur_node = q.front();
      // Keep track of when to increase depth of search
      if((time_to_increase_depth -= 1) == 0){
         if(++depth > d) break;
         time_to_increase_depth = q.size();
      }
      q.pop();

      // Iterate over outgoing edges of current node
      for(auto nbr_node : get_neighbors(cur_node)){
         edges++;
         // Do not add to queue again if node is already visited
         if(vis.find(nbr_node) != vis.end()) continue;
         q.push(nbr_node);
         vis.insert(nbr_node);
      }

      // Iterate over ingoing edges of current node
      if(!directed) continue;
      for(int it = 0; it < get_degree(cur_node); ++it){ //for(auto it : get_neighbors(cur)){  // Include for directed
         nbr_node = graph_r.e[graph_r.v[cur_node] + it];
         edges++;
         // Do not add to queue again if node is already visited
         if(vis.find(nbr_node) != vis.end()) continue;
         q.push(nbr_node);
         vis.insert(nbr_node);
      }
   }

   return vis;
}

void Graph::get_neighborhood_nauty(sparsegraph *new_graph, int &v, const int d){   
   std::unordered_map<int, int> map; // Maps each node in sg to new node in sg2
   int deg, pos, pos1, to, edges;

   // Get nodes in k-neighborhood
   std::unordered_set<int> nodes;
   if(!directed) nodes = get_neighborhood_nodes(v, d, edges);
   else get_neighborhood_nodes_directed(v, d, edges);
   
   int n = nodes.size();
   int m = SETWORDSNEEDED(n);

   // Initialize sub-graph
   new_graph->nv = n; // Set number of nodes
   new_graph->nde = 0;
   
   // Initialize map sg->sg2
   n = 0;
   for(auto it : nodes){
      map[it] = n;
      n++;
   }

   pos = 0;

   // For each node in the subgraph
   for(auto it : nodes){
      deg = 0;
      new_graph->v[map[it]] = pos; // Set position of node in sg2
      pos1 = graph.v[it]; 
      
      // Iterate over edges from it in original graph
      for(int i = 0; i < graph.d[it]; i++){
         to = graph.e[pos1 + i];

         // Check if edge is contained in subgraph
         if(nodes.find(to) == nodes.end()) continue;

         // Otherwise add edge to subgraph
         new_graph->nde++;
         new_graph->e[pos + deg] = map[to];
         deg++;
      }

      new_graph->d[map[it]] = deg;
      pos += deg;
   }
   v = map[v]; // Set v to corresponding node in new graph
}

Graph Graph::get_neighborhood(int &v, const int d){
   // Alloc
   SG_DECL(sub);
   SG_ALLOC(sub, graph.nv, graph.nde, "malloc");
   // Get neighborhood
   get_neighborhood_nauty(&sub, v, d);
   // Make graph object and return
   Graph graph = Graph(sub);
   return graph;
}

const std::set<int> Graph::get_giant_component_nodes(){
   size_t max_size = graph.vlen;
   std::set<int> node_set;
   std::queue<int> fringe;
   std::set<int> visited, bfs;
   size_t w, nbr;
   float threshold = GIANT_FRAC_THRESHOLD*graph.nv; // TODO: choose threshold

   // Iterate over nodes
   for(size_t v = 0; v < graph.vlen; v++){
      // Visit only nodes that have not been added yet
      if(visited.find(v) != visited.end())
         continue;
      bfs.clear();
      fringe.push(v);
      bfs.insert(v);
      visited.insert(v);
      
      // Do BFS from node
      while(!fringe.empty()){
         w = fringe.front();
         fringe.pop();
         for(auto nbr : get_neighbors(w)){
            if(visited.find(nbr) == visited.end()){
               fringe.push(nbr);
               bfs.insert(nbr);
            }
         }
         visited.insert(w);
      }

      // If component larger than threshold, add to node set
      if(bfs.size() > threshold){
         for(auto it : bfs)
            node_set.insert(it);
      }
   }
   return node_set;
}

void Graph::filter_giant_component(){
   nodes = get_giant_component_nodes();
}

const std::map<int, int> Graph::get_degree_distribution(){
   std::map<int, int> degrees;
   int degree;

   for(size_t i = 0; i < graph.nv; i++){
      degree = graph.d[i];
      
      if(degrees.find(degree) == degrees.end()) degrees.insert({degree, 1});
      else degrees[degree]++;
   }
   return degrees;
}

const std::map<int, int> Graph::get_degree_distribution_directed(){
   std::map<int, int> outdegs;
   int outdeg;

   for(size_t i = 0; i < graph.nv; i++){
      outdeg = graph.d[i];
      
      if(outdegs.find(outdeg) == outdegs.end()) outdegs.insert({outdeg, 1});
      else outdegs[outdeg]++;
   }
   return outdegs;
}

const long Graph::get_hash(){
   return hashgraph_sg(&graph, HASHKEY);
}

const void Graph::print_graph(){
   int n = get_number_nodes();

   for(int node = 0; node < n; node++){
      std::cout<<node<<": ";
      for(auto nbr : get_neighbors(node)){
         std::cout<<nbr<<", ";
      }
      std::cout<<std::endl;
   }
}