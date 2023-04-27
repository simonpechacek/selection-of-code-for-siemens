"""
This program removes all words that are in "slova.txt" and than prints
unused letters
"""
import sys

search_dir = [[1, 0], [1, 1], [0, 1]]


def find_word(osmismerka, words, row, col):
    height = len(osmismerka)
    width = len(osmismerka[0])
    found_words = []
    for dir in search_dir:
        r = row
        c = col
        found_word = None  # this should have start and end index of longest found word from starting index [row, col]
        good_words = words[:]
        string = ""
        while r < height and c < width:
            string += osmismerka[r][c]
            to_remove = []
            if len(good_words) == 0:
                break
            for word in good_words:
                if word == string:
                    found_word = [[row, col], [r, c]]
                if word.startswith(string):
                    continue
                else:
                    to_remove.append(word)

            for rem in to_remove:
                good_words.remove(rem)

            r += dir[0]
            c += dir[1]

        if found_word is not None:
            found_words.append(found_word)
    return found_words


def occupied_indexes(found_words):
    occ_indexes = []
    for word in found_words:
        r = word[0][0]
        c = word[0][1]
        occ_indexes.append([r, c])

        while r < word[1][0] and c < word[1][1]:
            r += 1
            c += 1
            occ_indexes.append([r, c])
        while r < word[1][0]:
            r += 1
            occ_indexes.append([r, c])

        while c < word[1][1]:
            c += 1
            occ_indexes.append([r, c])
    return occ_indexes


def print_result(osmismerka, occ_indexes):
    for r, row in enumerate(osmismerka):
        for c, letter in enumerate(row):
            if [r, c] not in occ_indexes:
                print(letter, end="")
    print()


def main():
    osmismerka = []
    words = []
    osm_path = sys.argv[1]
    words_path = sys.argv[2]
    with open(osm_path, "r") as f:
        for line in f.readlines():
            osmismerka.append(line.strip())
        f.close()

    with open(words_path, "r") as f:
        for line in f.readlines():
            words.append(line.strip())
        f.close()

    """words = sorted(words)
    for word in words:
        print(word)"""
    all_words = []
    for row in range(len(osmismerka)):
        for col in range(len(osmismerka[row])):
            new_words = find_word(osmismerka, words, row, col)
            if new_words:
                all_words += new_words

    occ_indexes = occupied_indexes(all_words)
    print_result(osmismerka, occ_indexes)


if __name__ == "__main__":
    main()
