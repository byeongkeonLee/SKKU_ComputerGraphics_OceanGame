# SKKU_ComputerGraphics_OceanGame
computer graphics OpenGL own 3D game


## 1. Playing Key
You can write the server IP via the keyboard keypad in the first scene.

    F1: Start the game as a Server
    F2: Start the game as a Client
    Keypad: Enter server IP address
    ↑, ↓ : Adjust the number of play round
    
  
In the second scene, you can play the game using the following key:

    W, A, S, D: Move the ship  
    (Ctrl + ) 1, 3, 4, space : (Reinforce) Skill  
    F1, F2, F3, F4 : Skill information of 1,3,4,space  
    Left Mouse: Normal Attack 
    
    
## 2. How to play?

A player should destroy the enemy's ship in a limited time. Crash the enemy's ship physically, attack the ship, or use skills. 
You can eat the wood boards or barrels floating in the ocean. The wood board will recover your HP and the barrel will increase your EXP.
If you have sufficient EXP, your level increase. Then, you can get a skill point that makes your skill advanced.

## 3. Techniques

### 1) Server-Client Programming

 It's a server-client 1vs1 PvP game. Therefore, we have developed server game mode and client game mode. For the user's convenience, we made a game starter (/bin_execute/cgblur).
 The server-client model diagram looks like the following.
 
 ![1](https://user-images.githubusercontent.com/43103079/72053115-bd304180-3309-11ea-9b23-35df36f9b53d.png)

 
 
### 2) Water - Reflection, Refraction

 For reality, we implemented ocean using frame buffers. It needs to be rendered 3 times like the following figure.
 
 ![2](https://user-images.githubusercontent.com/43103079/72053417-53646780-330a-11ea-8c55-816c931ec9be.png)
 
 First, we should take a reflection image by multiplying -1 on camera.eye.z as well as converting camera.at. At this time, only things whose z is above 0 should be rendered by using glClipDistance. In other words, the picture is temporally stored in the Reflection Frame buffer except underwater.
 Second, the Refraction framebuffer is similar to the reflection framebuffer. Just clip z which is above 0 to render only z<0. 
 Last, all models are rendered on the water texture blending refraction framebuffer and reflection framebuffer with the appropriate ratio.
 
 
 ### 3) Water - du,dv map
 
 To implement water flowing, our project uses dudv map and a normal map. The dudv map is a 2-direction vector to represent texture coordinate distortion. Likewise, the normal map has a 3-direction value representing a normal bump. Every point's normal vector value will change the normal value according to each 3-direction value.

 The below figures show dudv map and a normal map example.
 
 ![3](https://user-images.githubusercontent.com/43103079/72234129-df2e0a80-360e-11ea-9123-8f21aaf76c8c.png)

### 4) Fog system

 Visibility is calculated with the following formula: exp(-pow(distance * density, gradient)). The density and gradient are constants that are defined at compile time. Distance is the length between each object and the camera.
 
 Distance is calculated in the fragment shader, and visibility is calculated in the vertex shader. The lower the visibility, the grayer the color.
 
The below figure shows the game scene with a water and fog system applied.

![water](https://user-images.githubusercontent.com/43103079/73738137-3bf09100-4787-11ea-81ba-fa25f8f62bbc.png)


### 5) Terrain

 The terrain is generated from a height map which is bin/map/heightmap. The black color represents the lowest value. The blend map is used to blend the texture of the terrain. Each R,G,B value directly represents sand, stone, and snow texture. Then, the 1-R-G-B value is grass texture. As shown in the figure **4) Fog system**, you can also see how the terrain constructs the world. The following figures are the textures the game uses.

![map](https://user-images.githubusercontent.com/43103079/73738334-a7d2f980-4787-11ea-9979-958b0847b7ba.png)


### 6) Particles

 We implement a flame thrower with a particle system. The particle system is implemented by glAttribPointer to send particles' position and color data at once for high performance. In glsl, the layout is determined to get position and color data. Show the following figure:
 
 ![particle](https://user-images.githubusercontent.com/43103079/73739073-efa65080-4788-11ea-8543-4bb43d838d82.png)


### 7) Etc

 Other techniques are used but those are not important enough to explain here. Instead, I uploaded a PPT which I used in the project presentation in the lecture. Please refer to it. It might have some typos and grammatical errors. I appreciate your patience.
