class Register(object):
    def __init__(self, num):
        self.num = num

    def __str__(self):
        return "r" + str(self.num)

class VRegister(Register):

    def __init__(self, num):
        super(VRegister, self).__init__(num)
        self.live_range = [-1, -1]
        self.occurances = 1
        self.preg = None
        self.spilloff = 0
        self.uses = []

class PRegister(Register):
    def __init__(self, num):
        super(PRegister, self).__init__(num)
        self.vreg = None
