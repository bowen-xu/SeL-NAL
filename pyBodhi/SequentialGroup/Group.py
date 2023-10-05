from .SequentialGroup import *
from .SequentialGroup import Group as GroupBase
from depq import DEPQ
from typing import Callable

class Group(GroupBase):
    ''''''
    # t_now = 0
    def __init__(self, n_columns: int, n_nodes: int) -> None:
        super().__init__(n_columns, n_nodes)
        # self.tasks = DEPQ()


    # def update(self, t):
    #     self.t_now = t
    
    # def book(self, t: int, func: Callable, *args, kwargs={}):
    #     if t <= self.t_now:
    #         func(*args, **kwargs)
    #     else:
    #         self.tasks.insert((func, args, kwargs), int(t))
    
    # def activate_node(self, i_col, i_node):
    #     node: Node = self[i_col, i_node]
    #     node
    
    def __getitem__(self, idx) -> 'Node|Column':
        return GroupBase.__getitem__(self, idx)