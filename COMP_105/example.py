# COMP 105
# Fall 2018

import turtle
import math 

"""
PROJECT CRITERIA:
(1) use a FUNCTION CALL to draw a picture
(2) must use turtle graphics to create an image
(3) must have 3 distinct shapes (ex. circle, star, triangle)
(4) use at least one stamp (ex. turtle stamp)
(5) use at least three colors
"""

"""
running the module:
Fn + F5
-or-
"""

window = turtle.Screen()
window.bgcolor('yellow')

larry = turtle.Turtle() 
larry.shape('turtle')
larry.color('lightgreen')
larry.speed(0)

barry = turtle.Turtle()
barry.color('darkgreen')
barry.speed(0)
barry.shape('triangle') 
barry.setposition(-50,0)

jerry = turtle.Turtle()
jerry.color('pink')
jerry.speed(0)
jerry.shape('square')
jerry.setposition(50,0)

terry = turtle.Turtle()
terry.color('black')
terry.speed(0)
terry.shape('triangle')
terry.setposition(50,0)

def tree_leaves(t,b):
    b.penup()
    b.setposition(0,200)
    b.begin_fill()
    b.pendown()
    b.left(240)
    b.forward(350)
    b.left(120)
    b.forward(350)
    b.left(120)
    b.forward(350)
    b.hideturtle()
    b.end_fill()

    t.penup()
    t.hideturtle()
    t.setposition(150,100)
    t.pendown()
    t.showturtle()
    t.begin_fill()
    t.left(120)
    t.forward(300)
    t.stamp()
    t.left(120)
    t.forward(300)
    t.left(120)
    t.forward(300)
    t.end_fill()
    t.hideturtle()

def trunk (t):
    t.penup()
    t.setposition(25,-104)
    t.pendown()
    t.begin_fill()
    t.left(180)
    t.forward(50)
    t.left(90)
    t.forward(200)
    t.left(90)
    t.forward(50)
    t.left(90)
    t.forward(200)
    t.end_fill()
    t.hideturtle()

def star (t):
    t.penup()
    t.setposition(30,300)
    t.pendown()
    t.begin_fill()
    t.left(108)
    t.forward(100)
    t.left(144)
    t.forward(100)
    t.left(144)
    t.forward(100)
    t.left(144)
    t.forward(100)
    t.left(144)
    t.forward(100)
    t.end_fill()
    t.hideturtle()

tree_leaves(larry,barry)
trunk(jerry)
star(terry)

window.exitonclick() 
