# AI_shooter_battle_simulation


## Project Objective

The goal of this project is to demonstrate intelligent behavior of computer-controlled characters (AI).

![enter image description here](https://i.imgur.com/wqlyzpz.gif)


## Game Description

The game follows the specifications below:

-   Two teams of AI-controlled players compete against each other. Each team consists of two warriors and one support character (quartermaster). The objective is to eliminate the opposing team.
    
-   The characters navigate a maze containing rooms and passages.
    

## Tasks

1.  **Add Ammunition and Health Depots**
    
    -   Place two ammunition depots and two health stations randomly on the map.
        
2.  **Add Obstacles**
    
    -   Scatter obstacles across rooms to create hiding spots.
        
3.  **Define Character Resources and Goals**
    
    -   Each character has a limited supply of ammunition and health points (HP).
        
    -   Define AI behavior with the following goals:
        
        -   Seek engagement with enemy characters.
            
        -   Survive by retreating if health or ammunition drops below a certain threshold.
            
        -   Replenish ammunition when low by requesting supplies from the quartermaster.
            
        -   Heal when necessary by requesting aid from the quartermaster.
            

## Game Mechanics

1.  **Decision-Making Process**
    
    -   Characters continuously evaluate their state and prioritize objectives accordingly.
        
2.  **Randomized Character Traits**
    
    -   Assign each character a unique personality trait, such as aggression or cautiousness, affecting their combat behavior.
        
    -   Once a task is selected, execute pathfinding using the A* algorithm.
        
3.  **Finite State Machine (FSM) Implementation**
    
    -   Use an FSM to handle decision-making and character behavior transitions.
        
4.  **Team Collaboration**
    
    -   Characters can transfer supplies (ammo/health) to teammates via the quartermaster, who does not engage in combat.
        
    -   A character in need of resources must notify the quartermaster.
        
5.  **Combat Mechanics**
    
    -   Characters engage in combat when in the same room as an enemy.
        
    -   Implement two attack options: single-shot fire and grenade throws.
        
    -   Friendly fire is disabled.
        
6.  **Dynamic Movement Based on Safety Map**
    
    -   The game's safety map updates in real-time.
        
    -   Characters adjust their movement strategy based on evolving battlefield conditions.
        

This project integrates AI-driven decision-making, team collaboration, and strategic gameplay within a structured FSM framework.
