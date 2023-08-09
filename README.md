# OptimizerVisualization
This repository contains a 3D engine build from scratch that is capable of simulating downhill rolling of common deep learning optimizers 

Currently it can:
- Initialize the optimization landscape to a custom function

 <p align="center">
<img src="https://github.com/SzymonOzog/OptimizerVisualization/assets/58388001/48144c91-032f-4e48-a438-10319b79b285"  width="400" />
 </p>

- Edit the landscape post initialization
<p align="center">
<img src="https://github.com/SzymonOzog/OptimizerVisualization/assets/58388001/64200395-3c2d-4742-a6fc-4fab36bf81be"  width="400" />
</p>

- Optimize!
<p align="center">
<img src="https://github.com/SzymonOzog/OptimizerVisualization/assets/58388001/03960d33-0a6b-4849-a9da-33e118d36ec9"  width="400" />
</p>

## How to run it
Simply install the packages in requirements.txt and [g++](https://code.visualstudio.com/docs/cpp/config-mingw#_installing-the-mingww64-toolchain) and call `python run.py`

If you want to, you can edit the configuration file `config.json` or specify another one by runing with `-config=path`

## Controls
Move with WASD, press RMB to move the camera 

Keys from 1 - 5 change the input mode

1 - free movement

2 - landscape painting

3 - visualizer postion change

4 - running the optimization

5 - text input custom function to initialize landscape

## Why?
It obviously isn't a gamechanger in 3D or ML so why? The only purpose for creating this was to teach myself the basics of 3D graphics - how to actually rasterize triangles, calculate lightning, shade individual pixels, move the camera etc.
