import time
import random


class MergeSort(object):
    def __init__(self, VALUES, dt):
        self.VALUES = VALUES
        self.dt = dt
        self.sort_list = [i for i in range(VALUES)]
        random.shuffle(self.sort_list)
        
    def draw_graph(self, graph_obj):
        graph_obj.erase()
        
        graph_obj.draw_line((0, self.VALUES+5), (0, -5))
        graph_obj.draw_line((-5, 0), (self.VALUES*2, 0))
        
    
    def sort(self, graph_obj, window):
        
        # HERE YOU SORT 
        
        #AFTER EACH (CHANGE/LOOP/ITERATION)
        time.sleep(self.dt)
        self.draw_graph(graph_obj)
        
        # after you finish sorting
        
        window.write_event_value("-SORTED-", None)
    
