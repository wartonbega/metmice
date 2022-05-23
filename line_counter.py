import os

top_n = 0
top = ""


def lines_in(path):
    global top, top_n
    count = 0
    dirs = os.listdir(path)
    for i in dirs:
        if "." in i and path != "./":
            file = open(path+i, "r")

            cont = file.read()
            cont = cont.split("\n")
            count += len(cont)

            print(path+i, ":", len(cont))

            if len(cont) > top_n:
                top_n = len(cont)
                top = path+i

            file.close()

        elif i != "bin" and i != "makefile" and i != "linux_version":
            count += lines_in(path+i+"/")

    return count


print("lignes :", lines_in("src/") + lines_in("include/") + lines_in("lib/"))
print("top :", top, ":", top_n)
