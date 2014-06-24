Computer Graphics Project 2014
==

This is a project for Computer Graphics course in Computer Science @ DEI.pt

Goal
--
Create a simple game that teaches us how to work with OpenGL

Game Description
--
You control a player in a big open field. You must capture the missions to improve your player skills (attack, speed, jump, etc...). These missions spawn in a random position in the field. You must go there and stay within the radius for X amount of seconds. After being in the are for the required time, you improve your player skills and/or win some special powers. 

The difficulty of the game is created by having hundreds of enemies roaming around the field, colliding with each other and obstacles (created by the player or just trees). The player must avoid being around the enemies otherwise he will suffer damage.

Techniques
--
 - Quad tree for storing entities positions so they can collide with each other (they don't overlap)
 - X*Z Grid to store the collision map. 0 is empty, 1 is blocked.
 - Each entity (enemy) has is own AI (independent of each other). They target the player within a specific radius
