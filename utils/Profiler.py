import numpy as np
# from .Column import Column

class Profiler:
    ''''''
    def __init__(self, window=100) -> None:
        self.cnt_total = 0
        self.cnt_correct = 0
        self.pred_correct = np.full(window, False)
    
    # def observe1(self, pred, compare):
    #     ''''''
    #     if self.cnt_total < len(self.pred_correct):
    #         self.cnt_total += 1
    #     if len(pred) > 0:
    #         pred = sorted(pred, key=lambda x: -x[1].e)
    #         result = chr(compare+65) == Interpreter.get(Column, pred[0][0]._column._id)
    #     else:
    #         result = False
    #     if result: 
    #         self.cnt_correct += 1
    #     if self.pred_correct[0]:
    #         self.cnt_correct -= 1
    #     self.pred_correct[:-1] = self.pred_correct[1:]
    #     self.pred_correct[-1] = result
    #     acc = self.cnt_correct / self.cnt_total
    #     return acc

    def observe(self, pred: set, compare):
        ''''''
        if self.cnt_total < len(self.pred_correct):
            self.cnt_total += 1
        if len(pred) > 0:
            result = compare in pred
        else:
            result = False
        if result: 
            self.cnt_correct += 1
        if self.pred_correct[0]:
            self.cnt_correct -= 1
        self.pred_correct[:-1] = self.pred_correct[1:]
        self.pred_correct[-1] = result
        acc = self.cnt_correct / self.cnt_total
        return acc