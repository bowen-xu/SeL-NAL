try:
    from ..SequentialGroup import Group, Column, Node, Link, Bundle
except ValueError as e:
    from SequentialGroup import Group, Column, Node, Link, Bundle
from typing import List
import networkx as nx
from collections import defaultdict
import matplotlib.pyplot as plt

def draw_group(group: 'Group', figsize=(20, 4), ax=None):
    ''''''
    n_tasks = group.n_nodes
    g = nx.DiGraph()
    links_map = dict()

    for i, column in enumerate(group):
        column: Column
        for j, node in enumerate(column.nodes):
            node.update(group.ts)
            if node.a[-1] and node.p[-2]: color = "orange"
            elif node.a[-1]: color = "C8"
            else: color = "C0"
            if node.p[-1]: edgecolor = "red"
            # elif node._is_preactive[-2]: edgecolor = "C4"
            else: edgecolor = color #"C0"
            g.add_node((i,j), column=i, node=j, color=color, edgecolor=edgecolor)

    links = defaultdict(lambda: [None, None])
    for i, column in enumerate(group):
        column: Column
        for j, node in enumerate(column.nodes):
            node: Node
            id_link: Bundle = node.post_links
            if len(id_link) > 0:
                for _, l in id_link:
                    links_map[hash(l)] = l
                    links[hash(l)][0] = (i,j)
            id_link: List[Link] = node.ante_links
            if len(id_link) > 0:
                for _, l in id_link:
                    links_map[hash(l)] = l
                    links[hash(l)][1] = (i,j)
    links = [(link, id_link) for link, id_link in links.items() if id_link[0] is not None and id_link[1] is not None]

    for id_link, id_nodes in links:
        link: Link = links_map[id_link]
        if link.pv > group.thresh:
            width = min(link.pv, 1.0)*2
            # alpha = min(link.truth_predimp.c+0.1, 1.0)
            alpha = min(0.5, 1.0)
            g.add_edge(id_nodes[0], id_nodes[1], width=width, alpha=alpha)

    if ax is None:
        plt.figure(figsize=figsize)
        ax = plt.gca()
    ax.clear()

    color = [data["color"] for v, data in g.nodes(data=True)]
    edgecolor = [data["edgecolor"] for v, data in g.nodes(data=True)]
    pos = nx.multipartite_layout(g, subset_key="column")
    # nx.draw_networkx_nodes(g, pos, node_color=color, edgecolors=edgecolor, ax=ax)
    node_size = 100 # default: 300
    nx.draw_networkx_nodes(g, pos, ax=ax, node_size=node_size, node_color=color, edgecolors=edgecolor)

    edge_width = list(data["width"] for u, v, data in g.edges(data=True))
    edge_alpha = list(data["alpha"] for u, v, data in g.edges(data=True))
    nx.draw_networkx_edges(g, pos, width=edge_width, alpha=edge_alpha, ax=ax, node_size=node_size)