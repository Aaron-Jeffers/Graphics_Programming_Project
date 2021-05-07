Camera controls:

[Movement] WASD  - Note forward/backward movement is along the forward vector of the camera so is affected by pitch

[Pitch/Yaw] IJKL  

******IMPORTANT******* -> Pressing anything other than the controls listed above may cause a break point. I didn't have enough time to work out the kinks of the input system sorry.

Performance:

If performance is an issue lower quality models can be rendered. In [MainGame::initSystems()] -> [exampleMesh.loadModel(example string)] input a lower quality model if needed. See MainGame.h for list of different models that can be used.