# COMP 105
# Fall 2018

import turtle
import math 
import random

window = turtle.Screen()
window.bgcolor('pink')

painter = turtle.Turtle()
painter.pencolor("blue")

for i in range(500):
    if random.randint(1,5) == 1:
        painter.forward(20)
    if random.randint(1,5) == 2:
        painter.backward(20)
    if random.randint(1,5) == 3:
        painter.left(20)
    if random.randint(1,5) == 4:
        painter.right(20)

window.exitonclick() 
