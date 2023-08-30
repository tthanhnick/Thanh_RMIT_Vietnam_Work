# RMIT University Vietnam
# Course: COSC2429 Introduction to Programming
# Semester: 2021C
# Assignment: 1
# Author: Nguyen Tien Thanh (s3818111)
# Created date: 14/11/2021
# Last modified date: 15/11/2021


def find_split_80(integer_list):
    """Find the number in the list where exactly 80% of numbers in the list are equal to or smaller than it"""
    i = 0  # Define the variable count
    for i in integer_list:  # First loop to get the number in the list
        count = 0  # Define the variable count
        for j in integer_list:  # Second Loop to check the number meets the requirements
            if i >= j:  # if condition
                count = count + 1
        if count == 16:  # if it is at 80%
            break  # stop
    split_80 = i  # get and print the result
    return split_80  # return the wanted value


# RMIT University Vietnam
# Course: COSC2429 Introduction to Programming
# Semester: 2021C
# Assignment: 1
# Author: Nguyen Tien Thanh (s3818111)
# Created date: 14/11/2021
# Last modified date: 15/11/2021


import random  # import random library
import math  # import math library


def estimate_pi(N):
    """Estimate π using this algorithm"""
    count = 0  # Declare the variable count
    for i in range(N):  # Loop from 0 to N
        x = random.uniform(-1, 1)  # random number generator from -1 to 1
        y = random.uniform(-1, 1)  # random number generator from -1 to 1
        d = math.sqrt(x * x + y * y)  # Get the distance between x and y
        if d <= 1:  # if condition
            count = count + 1  # Count values
    estimated_pi = 4 * count / N  # Get the pi result
    return estimated_pi  # return the wanted value


# RMIT University Vietnam
# Course: COSC2429 Introduction to Programming
# Semester: 2021C
# Assignment: 1
# Author: Nguyen Tien Thanh (s3818111)
# Created date: 14/11/2021
# Last modified date: 15/11/2021

def flour_order(large_thick, large_thin, medium_thick, medium_thin):
    """Determine the provider"""
    total_pizza = large_thick * 0.55 + large_thin * 0.5 + medium_thick * 0.45 + medium_thin * 0.4  # Total pizza
    total_flour = total_pizza + total_pizza * 0.06  # Total flour
    if total_flour < 30:  # If condition to get promotion
        total_cost_a = total_flour * 30000 - total_flour * 30000 * 0.03
    else:  # Another condition to get promotion
        total_cost_a = total_flour * 30000 - total_flour * 30000 * 0.05
    if total_flour < 40:  # If condition to get promotion
        total_cost_b = total_flour * 31000 - total_flour * 31000 * 0.05
    else:  # Another condition to get promotion
        total_cost_b = total_flour * 31000 - total_flour * 31000 * 0.1
    print("We need to order ", total_flour, "kg of flour, which costs", total_cost_a, "VND if we buy from A and",
          total_cost_b, "VND if we buy from B.")  # Print out
    if total_cost_a <= total_cost_b:  # Compare price of two stores
        total_cost = total_cost_a  # Store total
        selected_provider = 'A'  # Provider
    else:  # Choose Another stores
        total_cost = total_cost_b  # Store total
        selected_provider = 'B'  # Provider
    return total_flour, selected_provider, total_cost  # return the wanted value


# RMIT University Vietnam
# Course: COSC2429 Introduction to Programming
# Semester: 2021C
# Assignment: 1
# Author: Nguyen Tien Thanh (s3818111)
# Created date: 14/11/2021
# Last modified date: 15/11/2021


import turtle  # import library


def draw_bar_chart(record_date, large_thick, large_thin, medium_thick, medium_thin):
    """Draw the bar chart"""
    s = 0  # Declare the variable s
    count = 0  # Declare the variable count
    # Initialize the turtle
    win = turtle.Screen()
    win.bgcolor("white")
    win.title("Bar Chart")
    bar_chart = turtle.Turtle()
    bar_chart.color("black")
    bar_chart.pensize(1)
    bar_chart.up()
    bar_chart.goto(-100, -100)
    bar_chart.down()
    # Set up the x axis
    x_axis = turtle.Turtle()
    x_axis.color("black")
    x_axis.pensize(1)
    x_axis.up()
    x_axis.goto(-150, -100)
    x_axis.down()
    # Set up the y axis
    y_axis = turtle.Turtle()
    y_axis.color("black")
    y_axis.pensize(1)
    y_axis.up()
    y_axis.goto(-150, -100)
    y_axis.down()
    # Write the date at the bottom of the bar
    bar_chart.left(90)
    bar_chart.up()
    bar_chart.backward(20)
    bar_chart.write(record_date)
    bar_chart.forward(20)
    bar_chart.down()
    # Use for loop to draw a bar chart
    for i in [large_thick, large_thin, medium_thick, medium_thin]:
        # Fill the color for each segment of the bar chart
        if count == 0:
            bar_chart.fillcolor("red")
        if count == 1:
            bar_chart.fillcolor("orange")
        if count == 2:
            bar_chart.fillcolor("brown")
        if count == 3:
            bar_chart.fillcolor("yellow")
        # Draw the bar graph
        bar_chart.begin_fill()
        s = s + i  # Increase the length of the bar
        count = count + 1  # Count each loop
        bar_chart.forward(s)
        bar_chart.right(90)
        bar_chart.forward(20)
        bar_chart.right(90)
        bar_chart.forward(i)
        bar_chart.right(90)
        bar_chart.forward(20)
        bar_chart.right(90)
        bar_chart.end_fill()
        if count == 2:  # Not go to the bottom to avoid replacing fill in color
            s = s - large_thick
        if count == 3:  # Not go to the bottom to avoid replacing fill in color
            s = s - large_thin
    # Label the bar graph
    bar_chart.up()
    bar_chart.forward(medium_thin+5)
    bar_chart.write(large_thick + large_thin + medium_thick + medium_thin)  # Total values
    x_axis.forward(120)  # Draw x axis
    y_axis.left(90)
    y_axis.forward(10 + large_thick + large_thin + medium_thick + medium_thin)  # Draw y axis
    win.exitonclick()  # Exit on click
