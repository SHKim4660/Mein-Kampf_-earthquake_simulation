import pygame
import sys
import numpy as np

pygame.init()
clock = pygame.time.Clock()
shouldClose = False

window = pygame.display.set_mode((640, 480))

q = np.array([[-3., -4., -6., -8.]])
q_dot = np.array([[0., 0., 0., 0.]])
a = np.zeros(4).reshape(q.shape)
a_prev = np.zeros(4).reshape(q.shape)
W = np.array([[1., 0., 0., 0.], [0., 1., 0., 0.], [0., 0., 1., 0.], [0., 0., 0., 1.]])

ks = 5
kd = 1.005

FPS = 120
dt = 1/FPS

while not shouldClose:
    Q = np.array([[0, 50, 0, 50]]).T
    
    # J = np.array([[2*q[0][0]],
    #               [2*q[0][1]]])
    # JT = np.array([[J[0][0], J[1][0]]])
    # J_dot = np.array([[2*q_dot[0][0]],
    #                   [2*q_dot[0][1]]])
    for i in range(1):
        J = np.array([[2*q[0][0], 2*q[0][1], 0, 0],
                      [2*(q[0][0] - q[0][2]), 2*(q[0][1] - q[0][3]),
                       -2*(q[0][0] - q[0][2]), -2*(q[0][1] - q[0][3])]])
        JT = J.T

        J_dot = np.array([[2*q_dot[0][0], 2*q_dot[0][1], 0, 0],
                          [2*(q_dot[0][0] - q_dot[0][2]), 2*(q_dot[0][1] - q_dot[0][3]),
                           -2*(q_dot[0][0] - q_dot[0][2]), -2*(q_dot[0][1] - q_dot[0][3])]])

        C = np.array([q[0][0]**2+q[0][1]**2-25,
                      (q[0][0] - q[0][2])**2+(q[0][1] - q[0][3])**2 - 25]).reshape(2, 1)

        C_dot = J@q_dot.T

        left = J@W@JT
        right = -J_dot@q_dot.T -J@W@Q
        damp = -ks*C-ks*C_dot

        lam = np.linalg.solve(left, right+damp)
        Q = Q + JT@lam
    F = Q

    # a_prev = np.array(a)
    a = F.T@W
    # # q_dot += a/60
    # # q += q_dot/60
    # q = q + q_dot/60 + 1/2*a_prev/60**2
    # q_dot = q_dot + (a_prev + a)/2/60
    q_dot += a*dt
    q += q_dot*dt

    window.fill((0, 0, 0))
    pygame.draw.circle(window, (255, 0, 0), (q[0][0]*10+320, q[0][1]*10+240), 3)
    pygame.draw.circle(window, (255, 0, 0), (q[0][2]*10+320, q[0][3]*10+240), 3)
    pygame.draw.circle(window, (0, 255, 0), (320, 240), 3)
    pygame.display.flip()

    clock.tick(FPS)

# 시발섹스
