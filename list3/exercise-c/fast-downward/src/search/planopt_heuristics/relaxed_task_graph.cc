#include "relaxed_task_graph.h"
#include "and_or_graph.h"

#include <deque>
#include <iostream>
#include <vector>

using namespace std;

namespace planopt_heuristics {
RelaxedTaskGraph::RelaxedTaskGraph(const TaskProxy &task_proxy)
    : relaxed_task(task_proxy),
      variable_node_ids(relaxed_task.propositions.size()) {
    /*
      TODO: add your code for exercise 2 (b) here. Afterwards
        - variable_node_ids[i] should contain the node id of the variable node for variable i
        - initial_node_id should contain the node id of the initial node
        - goal_node_id should contain the node id of the goal node
        - the graph should contain precondition and effect nodes for all operators
        - the graph should contain all necessary edges.
    */

    // Add propositions as variable nodes.
    for (Proposition p : relaxed_task.propositions) {
        variable_node_ids[p.id] = graph.add_node(NodeType::OR);
    }

    // Add initial node.
    initial_node_id = graph.add_node(NodeType::AND);
    for (PropositionID id : relaxed_task.initial_state) {
        graph.add_edge(variable_node_ids[id], initial_node_id);
    }

    // Add goal node.
    goal_node_id = graph.add_node(NodeType::AND);
    for (PropositionID id : relaxed_task.goal) {
        graph.add_edge(goal_node_id, variable_node_ids[id]);
    }

    // Add precondition and effect nodes for all operators.
    for (const RelaxedOperator &op : relaxed_task.operators) {
        NodeID preconditionNode = graph.add_node(NodeType::AND);
        for (PropositionID id : op.preconditions) {
            graph.add_edge(preconditionNode, variable_node_ids[id]);
        }

        NodeID effectNode = graph.add_node(NodeType::AND, op.cost);
        for (PropositionID id : op.effects) {
            graph.add_edge(variable_node_ids[id], effectNode);
        }

        graph.add_edge(effectNode, preconditionNode);
    }
}

void RelaxedTaskGraph::change_initial_state(const GlobalState &global_state) {
    // Remove all initial edges that where introduced for relaxed_task.initial_state.
    for (PropositionID id : relaxed_task.initial_state) {
        graph.remove_edge(variable_node_ids[id], initial_node_id);
    }

    // Switch initial state of relaxed_task
    relaxed_task.initial_state = relaxed_task.translate_state(global_state);

    // Add all initial edges for relaxed_task.initial_state.
    for (PropositionID id : relaxed_task.initial_state) {
        graph.add_edge(variable_node_ids[id], initial_node_id);
    }
}

bool RelaxedTaskGraph::is_goal_relaxed_reachable() {
    // Compute the most conservative valuation of the graph and use it to
    // return true iff the goal is reachable in the relaxed task.

    graph.most_conservative_valuation();
    return graph.get_node(goal_node_id).forced_true;
}

int RelaxedTaskGraph::additive_cost_of_goal() {
    // Compute the weighted most conservative valuation of the graph and use it
    // to return the h^add value of the goal node.

    // TODO: add your code for exercise 2 (c) here.
    graph.weighted_most_conservative_valuation();
    return graph.get_node(goal_node_id).additive_cost;
}

int RelaxedTaskGraph::ff_cost_of_goal() {
    // TODO: add your code for exercise 2 (e) here.
    graph.weighted_most_conservative_valuation();

    deque<NodeID> queue;
    queue.push_back(goal_node_id);

    unordered_set<NodeID> visited;

    while (!queue.empty()) {
        NodeID node_id = queue.front();
        queue.pop_front();

        if (visited.find(node_id) != visited.end()) {
            continue;
        }
        visited.insert(node_id);

        const AndOrGraphNode &node = graph.get_node(node_id);

        if (node.type == NodeType::OR) {
            queue.push_back(node.achiever);
        } else {
            for (NodeID succ: node.successor_ids) {
                queue.push_back(succ);
            }
        }
    }

    int cost = 0;

    for (NodeID node_id : visited) {
        const AndOrGraphNode &node = graph.get_node(node_id);
        cost += node.direct_cost;
    }

    return cost;
}

}
