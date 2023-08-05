import torch

class Optimizer():

    def __init__(self, config):
        self.pos_x = torch.tensor(0.0)
        self.pos_z = torch.tensor(0.0)

        params = [self.pos_x, self.pos_z]
        self.optimizer = eval(config['optimizer'])
    def step(self, grad_x, grad_z):
        self.optimizer.zero_grad()
        self.pos_x.grad = torch.tensor(grad_x)
        self.pos_z.grad = torch.tensor(grad_z)
        self.optimizer.step()
        return self.pos_x.item(), self.pos_z.item()
    
    def setPos(self, x, z):
        self.pos_x.data = torch.tensor(x)
        self.pos_z.data = torch.tensor(z)