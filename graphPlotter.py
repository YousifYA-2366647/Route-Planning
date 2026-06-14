#!/usr/bin/env python3
import turtle, math, sys

def load_graph():
    data = sys.stdin.read().replace("\ufeff", "").strip().split()
    if not data:
        print("No input provided!")
        sys.exit(1)
    
    it = iter(data)
    n = int(next(it))

    edges = []
    for a, b in zip(it, it):
        if a == "" or b == "":
            continue
        try:
            edges.append((int(a), int(b)))
        except Exception:
            continue
    
    return n, edges

def draw_graph(n, edges):
    screen = turtle.Screen()
    screen.title("Graph Visualizer")
    screen.setup(width = 1.0, height = 1.0)
    screen.tracer(0)
    tortuga = turtle.Turtle()
    tortuga.speed(0)
    tortuga.hideturtle()

    width = screen.window_width()
    height = screen.window_height()

    margin = 50
    usable_width = width - 2*margin
    usable_height = height - 2*margin

    size = min(usable_height, usable_width)

    radius = size / 2 - 20

    positions = []
    for i in range(n):
        angle = 2*math.pi*i/n
        x = radius*math.cos(angle)
        y = radius * math.sin(angle)
        positions.append((x, y))

    tortuga.color("black")
    for u,v in edges:
        turtle.width(10)
        x1, y1 = positions[u]
        x2, y2 = positions[v]

        tortuga.penup()
        tortuga.goto(x1, y1)
        tortuga.pendown()
        tortuga.goto(x2, y2)
    
    for i, (x, y) in enumerate(positions):
        tortuga.penup()
        tortuga.goto(x, y)
        tortuga.dot(10, "red")
    
    screen.update()
    turtle.done()

if __name__ == "__main__":
    n, edges = load_graph()
    draw_graph(n, edges)