import torch

class Optimizer():

    def __init__(self, **kwargs):
        self.pos_x = torch.tensor(0.0)
        self.pos_z = torch.tensor(0.0)
        self.optimizer = torch.optim.Adam([self.pos_x, self.pos_z], **kwargs)

    def step(self, grad_x, grad_z):
        self.optimizer.zero_grad()
        self.pos_x.grad = torch.tensor(grad_x)
        self.pos_z.grad = torch.tensor(grad_z)
        self.optimizer.step()
        return self.pos_x.item(), self.pos_z.item()
    
    def setPos(self, x, z):
        self.pos_x.data = torch.tensor(x)
        self.pos_z.data = torch.tensor(z)