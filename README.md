# PWMSmoothChange
Arduino library for controlling smooth change of PWM

This is library enabling smooth change of brightness of led. It utilitates Stevens's Power Law and relation between a perception of human eye and energy of light.

It calculates the PWM value respectively for the linear scale (e.g. from 0 to 100) as well as calculate time for changing the PWM to be longer in small PWM values and faster for higher so the changes seems to be smooth and linear.

It needs refining and adopting to good practices... So be forgiving please.
