# RMIT University Vietnam
# Course: COSC2429 Introduction to Programming
# Semester: 2021C
# Assignment: 1
# Author: Nguyen Tien Thanh (s3818111)
# Created date: 06/11/2021
# Last modified date: 06/11/2021


import turtle

win = turtle.Screen()
win.bgcolor("lightgreen")
tess = turtle.Turtle()
tess.color("blue")
tess.shape("turtle")
tess.pensize(3)
tess.up()
tito = turtle.Turtle()
tito.color("yellow")
tito.pensize(5)


def draw_sth_strange():
    """draw something strange"""
    for i in range(120, 240, 40):
        for j in range(12):
            tess.stamp()
            tess.backward(i)
            tess.left(30)
            tess.forward(i)
    tess.backward(240)


def draw_a_turtle():
    """draw a star"""
    tito.begin_fill()
    for k in range(5):
        tito.forward(150)
        tito.right(144)
    tito.end_fill()


def main():
    """main function"""
    draw_sth_strange()
    draw_a_turtle()
    win.delay(5000)
    win.exitonclick()
