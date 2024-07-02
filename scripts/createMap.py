from PIL import Image
import sys


def main():
    if (len(sys.argv) < 2):
        print("Usage: python createMap.py [in-file] [out-file]")
        return

    im = Image.open(sys.argv[1], "r").convert("RGBA")

    pix = im.load()
    data = bytes()

    for i in range(int(im.size[1]/64)):
        for j in range(int(im.size[0]/64)):
            if pix[j*64, i*64][3] != 0:
                data += b'\x01'
            else:
                data += b'\x00'

        data += b'\xff'

    name = sys.argv[1].split("/")
    name = name[len(name)-1].split(".")
    name = name[0]

    path = sys.argv[1].split(name)[0]
    path = f"{path}{name}.map"

    f = open(path, "wb")
    f.write(data)
    f.close()


if __name__ == "__main__":
    main()
