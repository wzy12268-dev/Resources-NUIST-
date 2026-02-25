from utils.priority_queue import PriorityQueue
import queue

def BFS(initial_state):
    """ BFS search"""
    frontier = queue.Queue() 
    frontier.put(initial_state)
    frontier_config = {}
    frontier_config[tuple(initial_state.config)] = True
    explored = set()
    nodes_expanded = 0
    max_search_depth = 0

    while not frontier.empty():
        state = frontier.get()
        print("****** State ******")
        state.display()
        explored.add(state.config)
        if state.is_goal():
            return (state,nodes_expanded,max_search_depth)
        
        nodes_expanded += 1
        for neighbor in state.expand(RLDU = False):
            if neighbor.config not in explored and tuple(neighbor.config) not in frontier_config:   
                frontier.put(neighbor)
                frontier_config[tuple(neighbor.config)] = True
                if neighbor.cost > max_search_depth:
                    max_search_depth = neighbor.cost
    return None

def DFS(initial_state):
    """DFS search"""
    frontier = queue.LifoQueue()
    frontier.put(initial_state)
    frontier_config = {}
    frontier_config[tuple(initial_state.config)] = True
    explored = set()
    nodes_expanded = 0
    max_search_depth = 0

    while not frontier.empty():
        state = frontier.get()
        print("****** State ******")
        state.display()
        explored.add(state.config)
        if state.is_goal():
            return (state,nodes_expanded,max_search_depth)

        nodes_expanded += 1
        for neighbor in state.expand():
            if neighbor.config not in explored and tuple(neighbor.config) not in frontier_config:
                frontier.put(neighbor)
                frontier_config[tuple(neighbor.config)] = True
                if neighbor.cost > max_search_depth:
                    max_search_depth = neighbor.cost
    return None

def A_STAR(initial_state,heuristic):
    """A * search"""
    frontier = PriorityQueue('min',heuristic)
    frontier.append(initial_state)
    frontier_config = {}
    frontier_config[tuple(initial_state.config)] = True
    explored = set()
    nodes_expanded = 0
    max_search_depth = 0

    while frontier:
        state = frontier.pop()
        print("****** State ******")
        state.display()
        explored.add(state)
        if state.is_goal():
            return (state,nodes_expanded,max_search_depth)

        nodes_expanded += 1
        for neigbhor in state.expand(RLDU= False):
            if neigbhor not in explored and tuple(neigbhor.config) not in frontier_config:
                frontier.append(neigbhor)
                frontier_config[tuple(neigbhor.config)] = True
                if neigbhor.cost > max_search_depth:
                    max_search_depth = neigbhor.cost
            elif neigbhor in frontier:
                if heuristic(neigbhor) < frontier[neigbhor]:
                    frontier.__delitem__(neigbhor)
                    frontier.append(neigbhor)
    return None