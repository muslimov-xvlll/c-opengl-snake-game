# Snake Game — C + OpenGL (Course Project)

## 📌 Description
This repository contains my course project: a full implementation of the classic Snake game written in **C** using **OpenGL** and **GLUT**.  
The game includes multiple difficulty levels, animated special fruits, shield mechanics, collision detection, full‑screen rendering, and a simple console menu before launch.

The project demonstrates work with low‑level rendering, game loops, timers, input handling, and state‑based gameplay logic.

---

## 🛠 Key Features

### ✔ Classic Snake Mechanics
- Movement in four directions  
- Growing when eating fruit  
- Collision detection  
- Game over screen with restart option  

### ✔ Multiple Fruit Types
- **Red fruit** — +1 length  
- **Golden fruit** — +5 score, limited lifetime, alpha animation  
- **Pink fruit** — resets snake length to 1  
- **Blue fruit** — activates temporary shield  

Each special fruit has:
- spawn chance  
- lifetime in frames  
- alpha‑blinking animation  

### ✔ Shield Mechanic
- Blue fruit grants temporary invulnerability  
- Shield has its own frame timer  
- Snake head changes color while shield is active  

### ✔ Three Difficulty Levels
1. No borders (teleport through edges)  
2. Borders around the field  
3. Borders + central obstacle  

### ✔ Rendering
- OpenGL immediate mode  
- GLUT for windowing and input  
- Full‑screen mode  
- Custom colors for each level  
- Alpha blending for animated fruits  
- Score rendering with outline effect  

### ✔ Input
- Arrow keys for movement  
- ESC — exit  
- R — restart after game over  

### ✔ Menu
Before the game starts, a console menu allows selecting the level.

---

## 🎯 Educational Goals
- Practice OpenGL rendering pipeline  
- Work with GLUT timers and callbacks  
- Implement game loop and state machine  
- Handle keyboard input  
- Use alpha blending and simple animations  
- Manage arrays and coordinate systems  
- Build a complete playable game in C  

---

## 📫 Contact
**Email:** muslimov.ramis13@gmail.com  
**Telegram:** https://t.me/muslimov_xvll

