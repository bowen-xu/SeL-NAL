#
# Automaticall generated file, do not edit!
#

"""
        Narsese module
    """
from __future__ import annotations
import narsese.narsese
import typing

__all__ = [
    "Budget",
    "Compound",
    "Connector",
    "Copula",
    "NarseseParser",
    "Punctuation",
    "Sentence",
    "Statement",
    "Task",
    "Term",
    "Truth",
    "parse",
    "parser"
]


class Budget():
    def __init__(self, p: float = 0.9, d: float = 0.9, q: float = 0.5) -> None: ...
    def __repr__(self) -> str: ...
    @property
    def d(self) -> float:
        """
        :type: float
        """
    @d.setter
    def d(self, arg0: float) -> None:
        pass
    @property
    def p(self) -> float:
        """
        :type: float
        """
    @p.setter
    def p(self, arg0: float) -> None:
        pass
    @property
    def q(self) -> float:
        """
        :type: float
        """
    @q.setter
    def q(self, arg0: float) -> None:
        pass
    pass
class Term():
    @typing.overload
    def __init__(self) -> None: ...
    @typing.overload
    def __init__(self, arg0: str) -> None: ...
    @staticmethod
    def __repr__(*args, **kwargs) -> typing.Any: ...
    @property
    def copula(self) -> object:
        """
        :type: object
        """
    @property
    def predicate(self) -> object:
        """
        :type: object
        """
    @property
    def subject(self) -> object:
        """
        :type: object
        """
    @property
    def terms(self) -> object:
        """
        :type: object
        """
    pass
class Connector():
    """
    Members:

      Conjunction : &&

      Disjunction : ||

      Product : *

      ParallelEvents : &|

      SequentialEvents : &/

      IntensionalIntersection : |

      ExtensionalIntersection : &

      ExtensionalDifference : -

      IntensionalDifference : ~

      Negation : --

      IntensionalSet : [

      ExtensionalSet : {

      IntensionalImage : \

      ExtensionalImage : /

      List : #
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    Conjunction: narsese.narsese.Connector # value = <Connector.Conjunction: 0>
    Disjunction: narsese.narsese.Connector # value = <Connector.Disjunction: 1>
    ExtensionalDifference: narsese.narsese.Connector # value = <Connector.ParallelEvents: 3>
    ExtensionalImage: narsese.narsese.Connector # value = <Connector.ExtensionalImage: 13>
    ExtensionalIntersection: narsese.narsese.Connector # value = <Connector.ExtensionalIntersection: 6>
    ExtensionalSet: narsese.narsese.Connector # value = <Connector.ExtensionalSet: 11>
    IntensionalDifference: narsese.narsese.Connector # value = <Connector.IntensionalDifference: 8>
    IntensionalImage: narsese.narsese.Connector # value = <Connector.IntensionalImage: 12>
    IntensionalIntersection: narsese.narsese.Connector # value = <Connector.IntensionalIntersection: 5>
    IntensionalSet: narsese.narsese.Connector # value = <Connector.IntensionalSet: 10>
    List: narsese.narsese.Connector # value = <Connector.List: 14>
    Negation: narsese.narsese.Connector # value = <Connector.Negation: 9>
    ParallelEvents: narsese.narsese.Connector # value = <Connector.ParallelEvents: 3>
    Product: narsese.narsese.Connector # value = <Connector.Product: 2>
    SequentialEvents: narsese.narsese.Connector # value = <Connector.SequentialEvents: 4>
    __members__: dict # value = {'Conjunction': <Connector.Conjunction: 0>, 'Disjunction': <Connector.Disjunction: 1>, 'Product': <Connector.Product: 2>, 'ParallelEvents': <Connector.ParallelEvents: 3>, 'SequentialEvents': <Connector.SequentialEvents: 4>, 'IntensionalIntersection': <Connector.IntensionalIntersection: 5>, 'ExtensionalIntersection': <Connector.ExtensionalIntersection: 6>, 'ExtensionalDifference': <Connector.ParallelEvents: 3>, 'IntensionalDifference': <Connector.IntensionalDifference: 8>, 'Negation': <Connector.Negation: 9>, 'IntensionalSet': <Connector.IntensionalSet: 10>, 'ExtensionalSet': <Connector.ExtensionalSet: 11>, 'IntensionalImage': <Connector.IntensionalImage: 12>, 'ExtensionalImage': <Connector.ExtensionalImage: 13>, 'List': <Connector.List: 14>}
    pass
class Copula():
    """
    Members:

      Inheritance : -->

      Similarity : <->

      Instance : {--

      Property : --]

      InstanceProperty : {-]

      Implication : ==>

      PredictiveImplication : =/>

      ConcurrentImplication : =|>

      RetrospectiveImplication : =\>

      Equivalence : <=>

      PredictiveEquivalence : </>

      ConcurrentEquivalence : <|>
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    ConcurrentEquivalence: narsese.narsese.Copula # value = <Copula.ConcurrentEquivalence: 11>
    ConcurrentImplication: narsese.narsese.Copula # value = <Copula.ConcurrentImplication: 7>
    Equivalence: narsese.narsese.Copula # value = <Copula.Equivalence: 9>
    Implication: narsese.narsese.Copula # value = <Copula.Implication: 5>
    Inheritance: narsese.narsese.Copula # value = <Copula.Inheritance: 0>
    Instance: narsese.narsese.Copula # value = <Copula.Instance: 2>
    InstanceProperty: narsese.narsese.Copula # value = <Copula.InstanceProperty: 4>
    PredictiveEquivalence: narsese.narsese.Copula # value = <Copula.PredictiveEquivalence: 10>
    PredictiveImplication: narsese.narsese.Copula # value = <Copula.PredictiveImplication: 6>
    Property: narsese.narsese.Copula # value = <Copula.Property: 3>
    RetrospectiveImplication: narsese.narsese.Copula # value = <Copula.RetrospectiveImplication: 8>
    Similarity: narsese.narsese.Copula # value = <Copula.Similarity: 1>
    __members__: dict # value = {'Inheritance': <Copula.Inheritance: 0>, 'Similarity': <Copula.Similarity: 1>, 'Instance': <Copula.Instance: 2>, 'Property': <Copula.Property: 3>, 'InstanceProperty': <Copula.InstanceProperty: 4>, 'Implication': <Copula.Implication: 5>, 'PredictiveImplication': <Copula.PredictiveImplication: 6>, 'ConcurrentImplication': <Copula.ConcurrentImplication: 7>, 'RetrospectiveImplication': <Copula.RetrospectiveImplication: 8>, 'Equivalence': <Copula.Equivalence: 9>, 'PredictiveEquivalence': <Copula.PredictiveEquivalence: 10>, 'ConcurrentEquivalence': <Copula.ConcurrentEquivalence: 11>}
    pass
class NarseseParser():
    pass
class Punctuation():
    """
    Members:

      Judgement : .

      Question : ?

      Goal : !

      Quest : @
    """
    def __eq__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __hash__(self) -> int: ...
    def __index__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    Goal: narsese.narsese.Punctuation # value = <Punctuation.Goal: 2>
    Judgement: narsese.narsese.Punctuation # value = <Punctuation.Judgement: 0>
    Quest: narsese.narsese.Punctuation # value = <Punctuation.Quest: 3>
    Question: narsese.narsese.Punctuation # value = <Punctuation.Question: 1>
    __members__: dict # value = {'Judgement': <Punctuation.Judgement: 0>, 'Question': <Punctuation.Question: 1>, 'Goal': <Punctuation.Goal: 2>, 'Quest': <Punctuation.Quest: 3>}
    pass
class Sentence():
    @staticmethod
    def __init__(*args, **kwargs) -> typing.Any: ...
    @staticmethod
    def __repr__(*args, **kwargs) -> typing.Any: ...
    pass
class Statement(Term):
    def __init__(self, arg0: Term, arg1: Copula, arg2: Term) -> None: ...
    @staticmethod
    def __repr__(*args, **kwargs) -> typing.Any: ...
    pass
class Task():
    @staticmethod
    def __repr__(*args, **kwargs) -> typing.Any: ...
    @property
    def sentence(self) -> SENTENCE::pSentence:
        """
        :type: SENTENCE::pSentence
        """
    @property
    def term(self) -> Term:
        """
        :type: Term
        """
    pass
class Compound(Term):
    def __init__(self, arg0: Connector, *args) -> None: ...
    @staticmethod
    def __repr__(*args, **kwargs) -> typing.Any: ...
    pass
class Truth():
    def __init__(self, f: float, c: float, k: float = 1.0) -> None: ...
    def __repr__(self) -> str: ...
    @property
    def c(self) -> float:
        """
        :type: float
        """
    @c.setter
    def c(self, arg0: float) -> None:
        pass
    @property
    def e(self) -> float:
        """
        :type: float
        """
    @property
    def f(self) -> float:
        """
        :type: float
        """
    @f.setter
    def f(self, arg0: float) -> None:
        pass
    @property
    def k(self) -> float:
        """
        :type: float
        """
    @k.setter
    def k(self, arg0: float) -> None:
        pass
    pass
def parse(line: str) -> object:
    pass
parser: narsese.narsese.NarseseParser
