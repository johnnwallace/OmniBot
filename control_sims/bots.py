import numpy as np
import pygame as pg

class OmniBot:
    def __init__(self, start, window):
        self.pos = start # position in global reference frame (x, y, angle)
        self.v = np.array([0, 0, 0]) # velocity in global reference frame (x, y, rate)
        self.window = window

    def update(self, dt):
        self.pos += self.v * dt

    def setVelocity(self, v):
        self.v = v

    def draw(self):
        pg.draw.circle(self.window, (0, 0, 255), self.pos[:-1], 10)
        arrow_poly = np.array([[0, -1], [0, 1], [20, 1], [20, 3], [30, 0], [20, -3], [20, -1]]) @ np.array([[np.cos(self.pos[2]), np.sin(self.pos[2])], [-np.sin(self.pos[2]), np.cos(self.pos[2])]])
        arrow_poly += np.array([self.pos[:-1], self.pos[:-1], self.pos[:-1], self.pos[:-1], self.pos[:-1], self.pos[:-1], self.pos[:-1]])
        pg.draw.polygon(self.window, (0, 0, 0), arrow_poly)

class Car:
    def __init__(self, start, window):
        self.L = 1 # wheelbase
        self.a = 0 # throttle
        self.velo = 0 # forward velocity
        self.steering = 0 # steering angle
        self.v = np.array([0, 0, 0]) # velocity in global reference frame (x, y, angle)
        self.pos = start # position in global reference frame (x, y, angle)
        self.window = window

    def update(self, dt):
        self.velo += self.a * dt
        self.v = np.array([self.velo * np.cos(self.pos[2]),
                           self.velo * np.sin(self.pos[2]),
                           self.velo * np.tan(self.steering)/self.L])
        self.pos += self.v

    def setControl(self, a, steering):
        self.a = a
        self.steering = steering

    def draw(self):
        pg.draw.circle(self.window, (255, 0, 0), self.pos[:-1], 10)
        arrow_poly = np.array([[0, -1], [0, 1], [20, 1], [20, 3], [30, 0], [20, -3], [20, -1]]) @ np.array([[np.cos(self.pos[2]), np.sin(self.pos[2])], [-np.sin(self.pos[2]), np.cos(self.pos[2])]])
        arrow_poly += np.array([self.pos[:-1], self.pos[:-1], self.pos[:-1], self.pos[:-1], self.pos[:-1], self.pos[:-1], self.pos[:-1]])
        pg.draw.polygon(self.window, (0, 0, 0), arrow_poly)
