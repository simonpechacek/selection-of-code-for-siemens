# this program finds tho longest symmetric row of numbers
# input: one row of ints
# output: index of first number in row, length of row


input_nums = list(map(int, input().split()))


def find_longest(nums: list):
    length = 0
    first_index = None
    found_rows = []
    for i in range(len(nums)):
        shift = 0
        length = 0
        for j in range(len(nums)):
            index = len(nums) - j - 1
            dji = (index - (i + shift))
            if dji == 0:
                if first_index is None:
                    first_index = i
                length = 2 * length + 1
                found_rows.append([first_index, length])
                first_index = None
                break
            elif dji == 1:
                if nums[i + shift] == nums[index]:
                    if first_index is None:
                        first_index = i
                    length += 1
                    found_rows.append([first_index, 2 * length])
                    first_index = None
                    break
                else:
                    first_index = None
                    break

            if nums[i + shift] == nums[index]:
                if first_index is None:
                    first_index = i
                shift += 1
                length += 1
            else:
                shift = 0
                first_index = None
                length = 0
    return found_rows


def get_max():
    maximum = input_nums[0]
    max_index = 0
    for i in range(len(input_nums)):
        if maximum < input_nums[i]:
            maximum = input_nums[i]
            max_index = i
    return max_index


def sum_seq(start_index, leng):
    summary = 0
    for i in range(start_index, start_index + leng):
        summary += input_nums[i]
    return summary


def get_longest(rows: list):
    maximum = None
    max_index = 0
    for i in range(len(rows)):
        if maximum is None or maximum < rows[i][1]:
            maximum = rows[i][1]
            max_index = i
        elif maximum == rows[i][1]:
            if sum_seq(rows[i][0], rows[i][1]) > sum_seq(rows[max_index][0], maximum):
                maximum = rows[i][1]
                max_index = i

    if maximum == 1:
        return get_max(), 1
    return rows[max_index]


all_rows = find_longest(input_nums)
best_row = get_longest(all_rows)
print(best_row[0], end=" ")
print(best_row[1])
