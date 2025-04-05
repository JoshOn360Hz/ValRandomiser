# Valorant Agent Randomizer

**Valorant Agent Randomizer** is a web-based game built using **Raylib** and **C**, designed to randomly select a Valorant agent based on a chosen role. It features a full Valorant-style user interface, dynamic animations, and a modern, responsive layout for an immersive experience.

## Features

# C
- Randomly selects a Valorant agent based on selected role: **Duelist**, **Controller**, **Initiator**, **Sentinel**, or **All**.
- Fully animated button interactions using **tweening** (lerp-based animations).
- Dynamic text wrapping for agent descriptions.
- Displays agent abilities, role, background, and face image.
# Web
- Dynamic light/dark mode toggle with local storage support.
- Custom mute and fullscreen controls.
- Loading spinner and progress feedback for web deployment.
- Minimalist console output area for Raylib logs.

## Built With

# C 
- **Raylib** - A simple and easy-to-use library to enjoy videogame programming.
- **C Language** - Core app logic.
# Web
- **HTML5 + CSS3** -  Frontend for the web version.
- **Bootstrap Icons** - Icons used in controls.
- **Google Fonts** - Makes the font nicer.

## Project Structure

**/resources/**
- **agentname_bg** – Background images for each agent.
- **agentname_face** – Face images for each agent.
- **/font.otf/** – Custom fonts used for titles and descriptions.

**main.c** – Raylib app.  
**shell.html** – web interface.  

