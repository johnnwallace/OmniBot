import numpy as np

class PID:
    def __init__(self, kp, ki, kd, lim):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.lim = lim
        self.last_error = 0
        self.cumulative_error = 0
        self.setpoint = 0

    def set(self, value):
        self.setpoint = value

    def update(self, state, dt):
        error = self.setpoint - state
        if error < 0.3 * self.setpoint:
            self.cumulative_error += error * dt
        derivative = (error - self.last_error) / dt

        output = self.kp * error + self.ki * self.cumulative_error + self.kd * derivative

        self.last_error = error

        return np.clip(output, -self.lim, self.lim)