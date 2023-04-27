import sys

path = sys.argv[1]

osmismerka = []

with open(path, "r") as f:
    for line in f.readlines():
        row = []
        for char in line.strip():
            row.append(char)

        osmismerka.append(row)

    f.close()

print(osmismerka)
