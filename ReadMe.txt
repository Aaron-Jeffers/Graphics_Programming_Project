Camera controls:

[Movement] WASD  - Note forward/backward movement is along the forward vector of the camera so is affected by pitch

[Pitch/Yaw] IJKL  


Performance:

If performance is an issue lower quality models can be rendered. In [MainGame::initSystems()] -> [exampleMesh.loadModel(example string)] input a lower quality model if needed. See MainGame.h for list of different models that can be used.