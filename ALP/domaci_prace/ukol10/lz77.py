"""
lz77 compression/decompression
search buffer = 16 nibbles
max repeating sequence = 5 nibbles

No repeating sequence => bit 0 + 4 bits of nibbles
Repeating sequence => bit 1 + 4 bits of offset from start of the buffer + 2 bits of (length - 2)
if 2 repeating sequences of same length -> choose the one with lower offset
MSB -> LSB
"""
import sys

hex_nums = {
    0: "0",
    1: "1",
    2: "2",
    3: "3",
    4: "4",
    5: "5",
    6: "6",
    7: "7",
    8: "8",
    9: "9",
    10: "A",
    11: "B",
    12: "C",
    13: "D",
    14: "E",
    15: "F"
}

hex_binary = {
    "0": "0000",
    "1": "0001",
    "2": "0010",
    "3": "0011",
    "4": "0100",
    "5": "0101",
    "6": "0110",
    "7": "0111",
    "8": "1000",
    "9": "1001",
    "A": "1010",
    "B": "1011",
    "C": "1100",
    "D": "1101",
    "E": "1110",
    "F": "1111"
}
binary_hex = {
    "0000": "0",
    "0001": "1",
    "0010": "2",
    "0011": "3",
    "0100": "4",
    "0101": "5",
    "0110": "6",
    "0111": "7",
    "1000": "8",
    "1001": "9",
    "1010": "A",
    "1011": "B",
    "1100": "C",
    "1101": "D",
    "1110": "E",
    "1111": "F"
}

binary_decimal = {
    "0000": 0,
    "0001": 1,
    "0010": 2,
    "0011": 3,
    "0100": 4,
    "0101": 5,
    "0110": 6,
    "0111": 7,
    "1000": 8,
    "1001": 9,
    "1010": 10,
    "1011": 11,
    "1100": 12,
    "1101": 13,
    "1110": 14,
    "1111": 15
}


def read_input():
    raw_data = []
    compress = None
    tmp = ""
    for line in sys.stdin:
        line = line.replace("\n", "")
        # print(len(line))
        convert_list = []
        if compress is None and (" " in line or "*" in line):
            compress = True
        elif compress is None:
            compress = False
        if compress:
            for char in line:
                convert_list.append(1 if char == "*" else 0)
            raw_data += convert_list
        else:
            convert_list = line.rstrip("\n")
            raw_data.append(convert_list)

    return raw_data, compress


def convert_to_nibbles(raw_data):
    """start at index 3 add 4 values to the left, add 4 """
    index = 3
    nibbles = []
    while index <= len(raw_data):
        binary_sum = 0
        for i in range(4):
            binary_sum += raw_data[index - i] * 2 ** i
        nibbles.append(hex_nums[binary_sum])
        index += 4
    return nibbles


def find_sequence(s_start, s_end, l_start, l_end, nibbles):
    max_length = 0
    min_offset = 0
    if s_start == -1:
        return 0, 0
    #print("start - end", s_start, s_end)
    for i in range(s_start, s_end + 1):
        length = 0
        index_search = i
        index_lookahead = l_start

        while (index_lookahead <= l_end) and (index_search <= s_end):
            #print("searching: ", nibbles[index_search:s_end + 1], nibbles[index_lookahead:l_end + 1])
            if nibbles[index_search] == nibbles[index_lookahead]:
                length += 1
            else:
                break
            index_lookahead += 1
            index_search += 1

        if length > max_length and length > 1:
            max_length = length
            min_offset = i - s_start
            if length == 5:
                break
    #print("best found:", min_offset, max_length)
    #print()
    return min_offset, max_length


def compress_to_binary(nibbles):
    """
    search buffer => 16 chars
    lookahead buffer => 5 chars
    """
    binary_data = ""
    search_buff_start = -1
    search_buff_end = -1
    lookahead_start = 0
    lookahead_end = 4
    while lookahead_start < len(nibbles):
        offset, length = find_sequence(search_buff_start, search_buff_end, lookahead_start, lookahead_end,
                                       nibbles)
        if length == 0:
            length = 1
            binary_data += "0"
            binary_data += hex_binary[nibbles[lookahead_start]]
            # binary_data += " "
        else:
            binary_data += "1"
            binary_data += hex_binary[hex_nums[offset]]
            binary_data += hex_binary[hex_nums[length - 2]][2:]
            # binary_data += " "
        if search_buff_end - search_buff_start == 15:
            search_buff_start += length
            search_buff_end += length
        else:
            search_buff_end += length
            search_buff_start = max(0, search_buff_end - 15)
        lookahead_start += length
        lookahead_end = min(len(nibbles) - 1, lookahead_start + 4)
        # print("S start: ", search_buff_start, "S end: ", search_buff_end,"L start: ", lookahead_start, "L end: ", lookahead_end)
        # print("offset: ", offset, "length: ", length)

    return binary_data


def binary_to_hex(binary_data):
    hex_code = ""
    binary = binary_data[0]
    length = len(binary_data)
    index = 1
    while index < length:
        if index % 4 == 0:
            hex_code += binary_hex[binary]
            binary = ""
        binary += binary_data[index]
        index += 1
    while len(binary) < 4:
        binary += "0"
    hex_code += binary_hex[binary]
    return hex_code


def hex_to_binary(raw_data):
    binary = ""
    for char in raw_data:
        binary += hex_binary[char]
    return binary


def decompress(binary_data):
    """
    if 1st bit is zero - next 4 bits - nibble
    if 1st bit is one - next 4 bits - offset, 2 bits length
    """
    index = 0
    hex_data = []
    search_buff_s = -1
    search_buff_e = -1
    while len(binary_data) - index > 5:
        if binary_data[index] == "0":
            # convert to hex
            hex_data.append(binary_hex[binary_data[index + 1: index + 5]])
            search_buff_e += 1
            search_buff_s = max(0, search_buff_e - 15)
            index += 5
        else:
            offset = binary_decimal[binary_data[index + 1: index + 5]]
            length = binary_decimal["00" + binary_data[index + 5: index + 7]] + 2
            for i in range(length):
                hex_data.append(hex_data[search_buff_s + offset + i])
            index += 7
            search_buff_e += length
            search_buff_s = max(0, search_buff_e - 15)
    return hex_data


def construct_image(hex_data, cols):
    image = []
    for j in range(0, len(hex_data), cols // 4):
        row = hex_data[j:j + cols // 4]
        row_string = ""
        for char in row:
            row_string += hex_binary[char]
        image.append(row_string)

    return image


def print_image(image):
    for row in image:
        for char in row:
            print(" " if char == "0" else "*", end="")
        print()


def check_output(output):
    with open("output_check.txt", "r") as f:
        correct = f.readline()
    f.close()
    for char in range(len(output)):
        if output[char] == correct[char]:
            continue
        else:
            print(output[char], correct[char])
            print("FALSE")
            print("at index", char)
            print("correct len: ", len(correct))
            print("my len: ", len(output))
            print(correct)
            return
    print("TRUE")


def main():
    raw_data, compress = read_input()

    if compress:
        nibbles = convert_to_nibbles(raw_data)
        # print(nibbles)
        binary_data = compress_to_binary(nibbles)
        # print(binary_data)
        hex_code = binary_to_hex(binary_data)
        #check_output(hex_code)
        print(hex_code)

    else:
        raw_data, columns = raw_data
        columns = int(columns)
        binary_data = hex_to_binary(raw_data)
        # print(binary_data)
        hex_data = decompress(binary_data)
        # print(hex_data)
        image = construct_image(hex_data, columns)
        # print(image)
        print_image(image)


if __name__ == "__main__":
    main()
