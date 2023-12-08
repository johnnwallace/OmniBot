import numpy as np
import pygame as pg

from bots import OmniBot, Car
from pid import PID

def getStates():
    dist = car.pos - omni.pos
    angle = np.arctan2(dist[1], dist[0])

    states = [angle + omni.pos[2], dist[2]] #LoS, angle diff, 
    return states



run = True
plot = False

# Set window
width, height = 800, 800
window = pg.display.set_mode((width, height))
pg.display.set_caption("Simulation")
time = 0

# Create clock
clock = pg.time.Clock()

# Initialize follower car
omni_start = np.array([width / 2, height / 2, np.pi/4])
car_start = omni_start + np.array([100, 100, 0])
omni = OmniBot(omni_start, window)
# omni.setVelocity(np.array([100, 0, 0]))

# initialize leader car
car = Car(car_start, window)
car.velo = 0
car.steering = 0.01

# Initialize controller
# controller = PID(1.2, 0.2, 0, 100)
# controller.set(3)

# # Initialize data
# data = Data(10000, 3, labels=["x", "y", "angle"])

while run:
    dt = clock.tick(60) / 1000
    time += dt

    window.fill((255, 255, 255))

    # Update body
    omni.update(dt)
    omni.draw()
    car.update(dt)
    car.draw()
    print(getStates())

    for event in pg.event.get():
        if event.type == pg.QUIT:
            run = False
        if event.type == pg.KEYDOWN:
            if event.key == pg.K_ESCAPE:
                run = False
                plot = True

    pg.display.flip()

pg.quit()
