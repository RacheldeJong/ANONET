#include "twinnode.h"

void Twinnodes::compute_twinnodes(){
   std::map<std::set<int>, int> values_o, values_c; // Maps set of neighbours to a node
   std::set<int> neighbours_open, neighbors_closed;
   std::map<std::set<int>, int>::iterator it_o, it_c;
   int class_tel = 0;

   twinnode_map.clear();
   twinnode_count = 0;

   // For each node, check if it has a twin node
   for(auto cur_node : graph->get_nodes()){
      
      // Skip nodes with high degree
      if(graph->get_degree(cur_node) > max_nbs_twin) continue;

      // Get values for node
      neighbours_open = graph->get_neighbors(cur_node);
      neighbors_closed = graph->get_neighbors(cur_node);
      neighbors_closed.insert(cur_node);

      // If yes, it has a twin node 
      if((it_o = values_o.find(neighbours_open)) != values_o.end()){
         twinnode_map[it_o->second].push_back(cur_node);
      }
      else if((it_c = values_c.find(neighbors_closed)) != values_c.end()){
         twinnode_map[it_c->second].push_back(cur_node);
      }
      // Otherwise, not: update target list and insert into all set
      else{
         values_o[neighbours_open] = class_tel;
         values_c[neighbors_closed] = class_tel;
         twinnode_map.push_back({cur_node});
         class_tel++;
      }
   }
}

void Twinnodes::compute_minimal_twins(){
   twin_minimal.clear();
   for(auto twin_set : twinnode_map){
      twin_minimal.push_back(twin_set.front());
   }
}

void Twinnodes::compute_node_to_twin(){
   node_to_twin = std::vector<int>(graph->get_number_nodes(), -1);
   int set_count = 0;
   for(auto twin_set : twinnode_map){
      for(auto node : twin_set){
         node_to_twin[node] = set_count;
      }
      set_count++;
   }
}

void Twinnodes::compute_nr_twins(){
   auto twins = get_twinnodes();
   twinnode_count = 0;

   for(auto it : twins){
      int size = it.size();
      if(size > 1) twinnode_count += size;
   }
}

void Twinnodes::compute(){
   compute_twinnodes();
   compute_minimal_twins();
   compute_node_to_twin();
   compute_nr_twins();
   initialized = true;
   non_twinnode_count = graph->get_number_nodes() - twinnode_count;
}

const std::vector< std::vector<int>> Twinnodes::get_twinnodes(){
   return twinnode_map;
}

const std::vector<int> Twinnodes::get_minimal_twins(){
   if(twin_minimal.size() == 1) compute();
   return twin_minimal;
}

const std::vector<int> Twinnodes::get_node_to_twin(){
   return node_to_twin;
}

const bool Twinnodes::is_initialized(){
   return initialized;
}

void Twinnodes::update_twinnodes(std::vector<int> changed_nodes){
   // TODO
}

const int Twinnodes::get_nr_twinnodes(){
   return twinnode_count;
}

const int Twinnodes::get_nr_nontwinnodes(){
   return non_twinnode_count;
}

const void Twinnodes::print_twinnodes(){
   auto twins = get_twinnodes();
   for(auto it : twins){
      for(auto it2 : it){
         std::cout<<it2<<", ";
      }
      std::cout<<std::endl;
   }
}