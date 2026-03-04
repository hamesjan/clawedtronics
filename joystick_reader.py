import pygame
import sys

# Raw calibration values
X_CENTER = -0.18
X_MIN = -0.80  # max left  -> 0
X_MAX = 0.42   # max right -> 100

Y_CENTER = -0.36
Y_MIN = -0.82  # max up   -> 100
Y_MAX = 0.16   # max down -> 0


def remap(raw, center, raw_min, raw_max, out_low, out_high):
    if raw <= center:
        t = (raw - raw_min) / (center - raw_min) if center != raw_min else 0.5
        val = out_low + t * (50 - out_low)
    else:
        t = (raw - center) / (raw_max - center) if raw_max != center else 0.5
        val = 50 + t * (out_high - 50)
    return max(0, min(100, round(val)))


pygame.init()
pygame.joystick.init()

if pygame.joystick.get_count() == 0:
    print("No joystick found. Plug in your Rockfire and try again.")
    sys.exit(1)

js = pygame.joystick.Joystick(0)
js.init()

print(f"Joystick: {js.get_name()}")
print(f"Axes: {js.get_numaxes()}  Buttons: {js.get_numbuttons()}  Hats: {js.get_numhats()}")
print("Reading input... Ctrl+C to quit\n")

clock = pygame.time.Clock()

try:
    while True:
        pygame.event.pump()

        raw_x = js.get_axis(0)
        raw_y = js.get_axis(1)

        x = remap(raw_x, X_CENTER, X_MIN, X_MAX, 0, 100)
        y = remap(raw_y, Y_CENTER, Y_MIN, Y_MAX, 100, 0)

        buttons = [js.get_button(i) for i in range(js.get_numbuttons())]
        hats = [js.get_hat(i) for i in range(js.get_numhats())]

        parts = []
        parts.append(f"X: {x:3d}  Y: {y:3d}")
        if any(buttons):
            pressed = [i for i, b in enumerate(buttons) if b]
            parts.append(f"Buttons: {pressed}")
        if any(h != (0, 0) for h in hats):
            parts.append(f"Hat: {hats}")

        print("\r" + "  |  ".join(parts).ljust(80), end="", flush=True)

        clock.tick(30)
except KeyboardInterrupt:
    print("\nDone.")
finally:
    pygame.quit()
