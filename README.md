# SKKU_ComputerGraphics_OceanGame
computer graphics opengl own 3D game


## 1. Playing Key
In First scene, you can write the server IP by keyboard keypad or number.

    F1 : Start the game as Server
    F2 : Start the game as Client
    KeyPad : Enter server IP address
    ↑, ↓ : play round up and down
    
  
In Second scene, you can game by using next key.  

    W, A, S, D : move the ship  
    (Ctrl + ) 1, 3, 4, space : (Reinforce) Skill  
    F1, F2, F3, F4 : Skill information of 1,3,4,space  
    Left Mouse : Normal Attack 
    
    
## 2. How to play?

Player should destroy other's ship in playing round time. Crash other player's ship, or attack the ship, or use skill. 
You can eat the woodboards or barrels on the ocean. Woodboard will recover your HP, and barrel will increase your EXP.
If you have sufficient EXP, level be up. Then, you can get skill point which make your skill be advanced.

## 3. Techniques

### 1) Server-Client Programming

 It's a server-client 1vs1 (P2P) PVP game. Therefore, we developed server game mode and client game mode. For user conveniencem, we made a game gurn on tool (/bin_execute/cgblur).
 The server client model digram is like below.
 
 ![1](https://user-images.githubusercontent.com/43103079/72053115-bd304180-3309-11ea-9b23-35df36f9b53d.png)

 
 
### 2) Water - Reflection, Refraction

 It's a ship war game. For reality, we implemented ocean using frame buffers. It need to render 3 times like next figure.
 
 ![2](https://user-images.githubusercontent.com/43103079/72053417-53646780-330a-11ea-8c55-816c931ec9be.png)
 
 First time, we should take a reflection image by multiplying -1 on camera.eye.z as well as converting camera.at. In this time, only things which z is above 0 should be rendered by using glClipDistance. Then, the picture is taken to Reflection Frame buffer.
 Second time, Refraction framebuffer is similar to reflection framebuffer. Just clip z which is above 0 to render only z<0. 
 Last, all models are rendered an the water texture is finally rendered with mixing refraction framebuffer, reflection framebuffer.
