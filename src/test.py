import minicv
from PIL import Image, ImageDraw

canvas = Image.new("RGB", (240, 240), "#0000ff")
draw = ImageDraw.Draw(canvas)
draw.rectangle(((0, 0), (100, 100)), fill="#00ff00")
draw.rectangle(((150, 150), (100, 100)), fill="#FF0000")


minicv.Image(canvas.tobytes(), w=240, h=240,bpp= 2)


# print(minicv.to_rgb24())

# print(img)

# display.show(mm)

# assert 255 == minicv.binary_to_grayscale(1)
# assert 0 == minicv.binary_to_grayscale(0)

assert (0, 0, 0) == minicv.binary_to_rgb(0)
assert (255, 255, 255) == minicv.binary_to_rgb(1)


print("binary_to_grayscale:1",minicv.binary_to_grayscale(1,nihao = False))
# print("binary_to_grayscale:0",minicv.binary_to_grayscale(0))

print("binary_to_rgb:0",minicv.binary_to_rgb(0))
print("binary_to_rgb:1",minicv.binary_to_rgb(1))

print("py_image_binary_to_lab",minicv.binary_to_lab(0))
print("py_image_binary_to_lab",minicv.binary_to_lab(1))


print(minicv.draw_line(0, 0, 100, 100, 0xFFFF66, 5))

print(minicv.draw_rectangle(100, 0, 100, 100, 0xCCCCFF, 5,fill = True))
print(minicv.draw_circle(120,200,50, c = 0xCCCCFF,thickness = 5,fill = False))

print("  asdsad: ",minicv.get_pixel(10,10))

print(minicv.find_lines())
# for i in minicv.find_lines():
# 	minicv.draw_line(i["x1"], i["y1"], i["x2"], i["y2"], 0x00FF66, 2)
print(minicv.find_line_segments())
# for i in minicv.find_line_segments():
# 	minicv.draw_line(i["x1"], i["y1"], i["x2"], i["y2"], 0xFFFF66, 2)


minicv.image_print()

img = minicv.to_rgb24()
mm = Image.frombytes(mode="RGB", size=(240,240), data=img[0], decoder_name="raw")
print(minicv.width())

mm.show()