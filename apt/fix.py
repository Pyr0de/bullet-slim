import sys

def main():
    if (len(sys.argv) != 2):
        return
    f = open(sys.argv[1], "+r")
    out = ""
    for i in f:
        a = i.split(" ", 1)
        b = a[1]
        a[1] = "[arch=amd64]"
        a.append(b)
        out += " ".join(a)
    f.seek(0)
    f.write(out)
    return

if __name__ == "__main__":
    main()
