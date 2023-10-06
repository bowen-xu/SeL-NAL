#
# Automaticall generated file, do not edit!
#

"""
        SequentialGroup module
    """
from __future__ import annotations
import pyBodhi.SequentialGroup.SequentialGroup
import typing
import pyBodhi.narsese
import pyBodhi.narsese.narsese

__all__ = [
    "Buffer",
    "Bundle",
    "Column",
    "Group",
    "Link",
    "Node",
    "narsese",
    "test"
]


class Buffer():
    pass
class Bundle():
    def __init__(self, arg0: Node, arg1: int, arg2: int) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    def add(self, arg0: Link, arg1: Node) -> typing.Tuple[Link, Link]: ...
    def remove(self, arg0: Link) -> bool: ...
    def update(self, arg0: Link, arg1: pyBodhi.narsese.narsese.Truth) -> None: ...
    @property
    def links(self) -> typing.Dict[Node, Link]:
        """
        :type: typing.Dict[Node, Link]
        """
    @property
    def links_ltb(self) -> typing.Set[Link]:
        """
        :type: typing.Set[Link]
        """
    @property
    def links_stb(self) -> typing.Set[Link]:
        """
        :type: typing.Set[Link]
        """
    @property
    def maxlink(self) -> Link:
        """
        :type: Link
        """
    @property
    def submaxlink(self) -> Link:
        """
        :type: Link
        """
    pass
class Column():
    def __getitem__(self, arg0: float) -> Node: ...
    def __init__(self, arg0: int) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __len__(self) -> int: ...
    def __repr__(self) -> str: ...
    def get_activities(self, amplifier: float = 1.0, expense: float = 0.5) -> typing.List[float]: ...
    def get_ambiguities(self) -> typing.List[float]: ...
    def select_nodes_by_activity(self, expense: float = 0.5, amplifier: float = 1.0) -> typing.List[typing.Tuple[float, Node]]: ...
    def select_nodes_by_ambiguity(self) -> typing.List[Node]: ...
    @property
    def mark(self) -> int:
        """
        :type: int
        """
    @mark.setter
    def mark(self, arg0: int) -> None:
        pass
    @property
    def nodes(self) -> typing.List[Node]:
        """
        :type: typing.List[Node]
        """
    pass
class Group():
    @typing.overload
    def __getitem__(self, arg0: typing.Tuple[int, int]) -> Node: ...
    @typing.overload
    def __getitem__(self, arg0: int) -> Column: ...
    def __getstate__(self) -> tuple: ...
    def __init__(self, arg0: int, arg1: int) -> None: ...
    def __iter__(self) -> typing.Iterator: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, arg0: tuple) -> None: ...
    def activate(self, arg0: int) -> typing.Set[Node]: ...
    @staticmethod
    def build_links(arg0: typing.List[Node], arg1: typing.List[Node]) -> None: ...
    def nop(self) -> None: ...
    @property
    def columns(self) -> typing.List[Column]:
        """
        :type: typing.List[Column]
        """
    @property
    def n_columns(self) -> int:
        """
        :type: int
        """
    @property
    def n_nodes(self) -> int:
        """
        :type: int
        """
    @property
    def p_minus(self) -> float:
        """
        :type: float
        """
    @p_minus.setter
    def p_minus(self, arg0: float) -> None:
        pass
    @property
    def p_minus2(self) -> float:
        """
        :type: float
        """
    @p_minus2.setter
    def p_minus2(self, arg0: float) -> None:
        pass
    @property
    def p_plus(self) -> float:
        """
        :type: float
        """
    @p_plus.setter
    def p_plus(self, arg0: float) -> None:
        pass
    @property
    def thresh(self) -> float:
        """
        :type: float
        """
    @thresh.setter
    def thresh(self, arg0: float) -> None:
        pass
    @property
    def ts(self) -> int:
        """
        :type: int
        """
    pass
class Link():
    def __hash__(self) -> int: ...
    def __repr__(self, interpreter: capsule = None) -> str: ...
    @property
    def ante_node(self) -> Node:
        """
        :type: Node
        """
    @property
    def post_node(self) -> Node:
        """
        :type: Node
        """
    @property
    def pv(self) -> float:
        """
        :type: float
        """
    @property
    def ts_activate(self) -> int:
        """
        :type: int
        """
    @ts_activate.setter
    def ts_activate(self, arg0: int) -> None:
        pass
    pass
class Node():
    def __contains__(self, arg0: Link) -> bool: ...
    def __hash__(self) -> int: ...
    @typing.overload
    def __init__(self, word: str, n_ltb: int = 100, n_stb: int = 100) -> None: ...
    @typing.overload
    def __init__(self, n_ltb: int = 100, n_stb: int = 100) -> None: ...
    def __repr__(self, interpreter: capsule = None) -> str: ...
    @staticmethod
    def connect(arg0: Node, arg1: Node, arg2: float) -> Link: ...
    def connect_to(self, node2: Node, pv: float = 0.3) -> Link: ...
    @typing.overload
    def contains_antelink(self, arg0: Link) -> bool: ...
    @typing.overload
    def contains_antelink(self, arg0: Node) -> bool: ...
    @typing.overload
    def contains_postlink(self, arg0: Link) -> bool: ...
    @typing.overload
    def contains_postlink(self, arg0: Node) -> bool: ...
    @typing.overload
    def remove_antelink(self, arg0: Link) -> bool: ...
    @typing.overload
    def remove_antelink(self, arg0: Node) -> bool: ...
    @typing.overload
    def remove_link(self, arg0: Link) -> bool: ...
    @typing.overload
    def remove_link(self, arg0: Node) -> bool: ...
    @typing.overload
    def remove_postlink(self, arg0: Link) -> bool: ...
    @typing.overload
    def remove_postlink(self, arg0: Node) -> bool: ...
    def update(self, arg0: int) -> None: ...
    @property
    def a(self) -> typing.List[bool]:
        """
        :type: typing.List[bool]
        """
    @a.setter
    def a(self, arg0: typing.List[bool]) -> None:
        pass
    @property
    def activity(self) -> float:
        """
        :type: float
        """
    @property
    def activity_pred(self) -> float:
        """
        :type: float
        """
    @property
    def ambiguity(self) -> float:
        """
        :type: float
        """
    @property
    def ante_links(self) -> Bundle:
        """
        :type: Bundle
        """
    @property
    def column(self) -> Column:
        """
        :type: Column
        """
    @property
    def p(self) -> typing.List[bool]:
        """
        :type: typing.List[bool]
        """
    @p.setter
    def p(self, arg0: typing.List[bool]) -> None:
        pass
    @property
    def post_links(self) -> Bundle:
        """
        :type: Bundle
        """
    pass
def test() -> None:
    pass
