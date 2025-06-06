from SequentialGroup import Column, Group, Node, Link
from utils.dataset import generate_dataset as _generate_dataset
from pathlib import Path
import pickle
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import matplotlib.animation as animation
from tqdm import tqdm
from utils.Profiler import Profiler
from typing import Set
from utils.draw_group import draw_group

np.random.seed(137)
sparse_rate = 1.00  # 0.05
mu = 0.2
sigma = 0.01


def generate_dataset(length: int=4, number: int=2, n_train=1000, n_test=1000, n_chars=26):
    '''
    Args:
        length (int): the length of each subsequence
        number (int): the number of subsequences
    '''
    from ordered_set import OrderedSet
    length = int(length)
    number = int(number)
    assert (number >= 1) and (length >= 4)
    seqs = [] # subsequences
    for _ in range(number):
        samples = OrderedSet([chr(i) for i in range(65, n_chars+65)])
        seq = list(np.random.choice(samples, length))
        seqs.append(seq)
    D_train, D_test, _dictionary = _generate_dataset(seqs, n_train, n_test, n_chars=n_chars)
    # _bias = ord('A')
    # D_train = [ord(c)-_bias for c in D_train]
    # D_test = [ord(c)-_bias for c in D_test]
    return D_train, D_test, _dictionary



def test_capacity(length, number, n_nodes=16, n_chars=26, observe_all=True, loose=True):
    ''''''
    # Prepare the dataset
    root_cache = Path('./cache')
    root_cache.mkdir(parents=True, exist_ok=True)
    file_cache = root_cache/f"dataset_capacity-test_{length}_{number}_{n_chars}.pkl"
    if not file_cache.exists():
        D_train, D_test, _dictionary  = generate_dataset(length, number, n_chars=n_chars, n_train=length*number*200)
        with open(file_cache, 'wb') as f:
            pickle.dump((D_train, D_test, _dictionary), f)
    else:
        with open(file_cache, "rb") as f:
            D_train, D_test, _dictionary = pickle.load(f)
    
    # Create the model
    chars = [chr(i) for i in range(65,65+n_chars)]
    g = Group(len(chars), n_nodes)
    g.thresh = 0.8
    g.p_plus = 2#1.95
    g.p_minus = 0.99#0.9
    g.p_minus2 = 0.001

    for idx, column in enumerate(g.columns):
        column.mark = idx

    data = D_train[:length*number*400]
    print(len(data))
    print(data[:20])
    accuracies = []
    n_anticipations = []

    duration = length*2-2

    profiler = Profiler(100)
    for i, (idx, idx_next) in enumerate((tqdm(zip(data[:-1], data[1:]), total=len(data)-1))):
        candidates: Set[Node] = g.activate(idx)
        anticipations = set()
        if loose:
            for candidate in candidates:
                anticipations.add(candidate.column.mark)
        else:
            if len(candidates) > 0:
                candidate = max(candidates, key=lambda node: node.activity_pred)
                anticipations.add(candidate.column.mark)

        if not observe_all and not (((remainder := i%duration) > 0) and (remainder < length-1)):
            continue

        acc = profiler.observe(anticipations, idx_next)
        accuracies.append(acc)
        n_anticipations.append(len(anticipations))
        
    return accuracies, g


pair = (14,20)
accuracies, g = test_capacity(*pair, 10, observe_all=True, loose=True)
plt.figure()
plt.title(f"length={pair[0]}, #sequences={pair[1]}")
plt.plot(accuracies)
plt.axhline(0.5, c='r', alpha=0.5)
# draw_group(g)
plt.show()
