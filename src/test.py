import minicv
from PIL import Image, ImageDraw
import numpy as np

canvas = Image.new("RGB", (240, 240), "#0000ff")
draw = ImageDraw.Draw(canvas)
draw.rectangle(((0, 0), (100, 100)), fill="#00ff00")
draw.rectangle(((150, 150), (100, 100)), fill="#FF0000")


minicv.Image(canvas.tobytes(), w=240, h=240,bpp= minicv.IMAGE_BPP_RGB565)


# print(minicv.to_rgb24())

# print(img)

# display.show(mm)

# assert 255 == minicv.binary_to_grayscale(1)
# assert 0 == minicv.binary_to_grayscale(0)

assert (0, 0, 0) == minicv.binary_to_rgb(0)
assert (255, 255, 255) == minicv.binary_to_rgb(1)


print("binary_to_grayscale:1",minicv.binary_to_grayscale(1))
# print("binary_to_grayscale:0",minicv.binary_to_grayscale(0))

print("binary_to_rgb:0",minicv.binary_to_rgb(0))
print("binary_to_rgb:1",minicv.binary_to_rgb(1))

print("py_image_binary_to_lab",minicv.binary_to_lab(0))
print("py_image_binary_to_lab",minicv.binary_to_lab(1))


print(minicv.draw_line(0, 0, 100, 100, 0xFFFF66, 5))

print(minicv.draw_rectangle(100, 0, 100, 100, 0xCCCCFF, 5,fill = True))
print(minicv.draw_circle(120,200,50, c = 0xCCCCFF,thickness = 5,fill = True))

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
# while True:

img = Image.open("/home/nihao/nihao.png")
# img.show()
print(img.format,img.size,img.mode)
img = img.convert('RGB')
print(img.format,img.size,img.mode)

minicv.Image(img.tobytes(), w=img.width, h=img.height ,bpp= minicv.IMAGE_BPP_RGB565)

# print(img.tobytes())
# mda = np.array(img)
# print(mda)
# mka = minicv.to_grayscale()
# print(mka)

# mm = Image.frombytes(mode="L", size=(260,260), data=mka[0], decoder_name="raw")
# mm.show()

print("find_qrcodes",minicv.find_qrcodes())

# imgb = minicv.to_rgb24()
# mm = Image.frombytes(mode="RGB", size=img.size, data=imgb[0], decoder_name="raw")
# mm.show()






