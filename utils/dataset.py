import numpy as np
from ordered_set import OrderedSet
from pathlib import Path
import pickle
import hashlib

def generate_dataset(seqs, n_train=10000, n_test=10000, seed=137, exclude_sample=False, randoms=True, cache_root='./cache', regenerate=False, n_chars=26):
    cache_root = Path(cache_root)
    cache_root.mkdir(parents=True, exist_ok=True)
    s = f"{n_train}, {n_test}, seqs={seqs}, n_chars={n_chars}"
    # get hash-value of seqs
    md5_hash = hashlib.md5()
    md5_hash.update(s.encode('utf-8'))
    hash_hex = md5_hash.hexdigest()
    hash_value = int(hash_hex, 16)
    # the hash-value is part of the file's name
    filepath = cache_root/(str(hash_value)+".pkl")
    
    dictionary = {ele: i for i, ele in enumerate(set([ele for seq in seqs for ele in seq]))}
    n_chars = max(n_chars, len(dictionary))
    if not filepath.exists() or regenerate:
        np.random.seed(seed)
        samples = OrderedSet([i for i in range(n_chars)])
        if exclude_sample:
            samples_seqs = set(dictionary.keys())
            samples -= samples_seqs
        seqs = [[dictionary[ele] for ele in seq] for seq in seqs]
        # print(seqs)
        def produce_sequence(seqs, n=10000):
            sequence = []
            for _ in range(n):
                seq = seqs[(np.random.choice(len(seqs), 1))[0]]
                if randoms:
                    seq = seq + list(np.random.choice(samples, len(seqs[0])-2))
                sequence.extend(seq)
            return sequence

        D_train = produce_sequence(seqs, n_train)
        D_test = produce_sequence(seqs, n_test)

        with open(str(filepath), 'wb') as f:
            pickle.dump({"D_train": D_train, "D_test": D_test}, f)
    else:
        with open(str(filepath), 'rb') as f:
            Dataset = pickle.load(f)
            D_train = Dataset["D_train"]
            D_test = Dataset["D_test"] 

    return D_train, D_test, dictionary