input_string = input()

keywords = [
    "nula", "jedna", "dva", "tri", "ctyri", "pet", "sest", "sedm", "osm", "devet",
    "deset", "jedenact", "dvanact", "trinact", "ctrnact", "patnact", "sestnact",
    "sedmnact", "osmnact", "devatenact", "dvacet", "tricet", "ctyricet", "padesat",
    "sedesat", "sedmdesat", "osmdesat", "devadesat", "sto", "dveste", "trista", "ctyrista",
    "petset", "sestset", "sedmset", "osmset", "devetset", "tisic","desetin", "setin",
    "tisicin", "desetitisicin", "stotisicin", "miliontin", "cela", "cele", "celych", "tisice"
]

stovky = {
    0: "",
    1: "sto",
    2: "dveste",
    3: "trista",
    4: "ctyrista",
    5: "petset",
    6: "sestset",
    7: "sedmset",
    8: "osmset",
    9: "devetset"
}
desitky = {
    0: "",
    2: "dvacet",
    3: "tricet",
    4: "ctyricet",
    5: "padesat",
    6: "sedesat",
    7: "sedmdesat",
    8: "osmdesat",
    9: "devadesat"
}
mensi_nula = {
    0.1: "desetin",
    0.01: "setin",
    0.001: "tisicin",
    0.0001: "desetitisicin",
    0.00001: "stotisicin",
    0.000001: "miliontin"
}


def formatNums(string: str):
    before = []
    after = []
    before_dot = True
    for lt in string:

        if lt != ".":
            if before_dot:
                before.append(int(lt))
            else:
                after.append(int(lt))
        else:
            before_dot = False
    return before, after


def split_list(after: list):
    if len(after) > 6:
        print("ERROR")
        quit()
    lst1, lst2 = [], []

    for item in after[::-1]:
        if len(lst1) < 3:
            lst1.insert(0, item)
        else:
            lst2.insert(0, item)
    return lst2, lst1


def zero_to_thousand(before: list):
    vel = len(before)
    if vel > 3:
        print("ERROR")
        quit()
    num_string = ""
    for i in range(vel):
        index = vel - i
        if index == 3:
            num_string += stovky[before[i]]
        elif index == 2:
            if before[i] > 1:
                num_string += desitky[before[i]]
            elif before[i] == 1:
                temp_index = 10 + before[i + 1]
                num_string += keywords[temp_index]
                break
        else:
            if before[i] != 0:
                num_string += keywords[before[i]]
            else:
                if len(before) == 1:
                    num_string += keywords[before[i]]
    return num_string


def numToStr(before, after):
    output = ""
    before_str = zero_to_thousand(before)
    output += before_str

    if len(after) > 0:

        if output in ["nula", "jedna"]:
            output += "cela"
        elif output in ["dva", "tri", "ctyri"]:
            output += "cele"
        else:
            output += "celych"

        after1, after2 = split_list(after)
        after1_str = ""
        if len(after1) > 0:
            if len(after1) == 1 and after1[-1] == 0:
                pass
            else:
                after1_str = zero_to_thousand(after1)
                if after1_str == "jedna":
                    after1_str = "tisic"
                elif after1_str in ["dva", "tri", "ctyri"]:
                    after1_str += "tisice"
                elif after1_str != "":
                    after1_str += "tisic"

        after2_str = zero_to_thousand(after2)
        after2_str += mensi_nula[(10 ** (-len(after)))]
        output += after1_str
        output += after2_str

    print(output)


def split_string(string: str):
    for split in ["cela", "cele", "celych"]:
        splitted = string.split(split)
        if len(splitted) > 1:
            return splitted[0], splitted[1]
    return string, ""


def split_below_zero(string: str):
    for split in ["tisice", "tisic"]:
        splitted = string.split(split)
        if len(splitted) > 1:
            if len(splitted[0]) == 0:
                return "jedna", splitted[1], True
            return splitted[0], splitted[1], True
    return string, "", False


def strToNum(input: str):
    before, after = split_string(input)
    before = construct_number(before)
    if after != "":
        factor, after = split_num_factor(after)
        after1, after2, times_thousand = split_below_zero(after)
        if after2 != "":
            after1 = construct_number(after1) * 1000
            after2 = construct_number(after2)
            total = (after1 + after2) * factor
            if total > 1:
                print("ERROR")
                quit()
            else:
                total = before + total
                print(f"%.{get_fpu(factor)}f" % total)
        else:
            after1 = construct_number(after1)
            if times_thousand:
                total = after1 * 1000 * factor
            else:
                total = after1 * factor

            if total >= 1:
                print("ERROR")
                quit()
            else:
                total = before + total
                print(f"%.{get_fpu(factor)}f" % total)

    else:
        print(str(before))


def get_fpu(factor):
    count = 0
    while factor != 1:
        factor *= 10
        count += 1
    return count


def construct_number(string: str):
    index = 0
    number = 0
    while len(string) != 0:
        found = False
        if index == 0:
            for i, keyword in enumerate(keywords[:20]):
                if string.endswith(keyword):
                    number += i
                    index += 1
                    string = string[:len(string) - len(keyword)]
                    found = True
                    break
            if not found:
                index += 1
        elif index == 1:
            for key in desitky:
                if desitky[key] == "":
                    continue
                if string.endswith(desitky[key]):
                    number += key * 10
                    index += 1
                    string = string[:len(string) - len(desitky[key])]
                    found = True
                    break
            if not found:
                index += 1
        elif index == 2:
            for key in stovky:
                if stovky[key] == "":
                    continue
                if string.endswith(stovky[key]):
                    number += key * 100
                    string = string[:len(string) - len(stovky[key])]
                    index += 1
                    found = True
                    break
            if not found:
                index += 1
        else:
            print("ERROR")
            quit()
    return number


def split_num_factor(string):
    times = 0
    factors = list(mensi_nula.values())
    for factor in factors[::-1]:
        if string.endswith(factor):
            if factor == "setin" and string.endswith("desetin"):
                factor = "desetin"
            times = factor
            string = string[:len(string) - len(factor)]
    for key in mensi_nula:
        if mensi_nula[key] == times:
            return key, string
    print("ERROR")
    quit()


def validate_input(string: str):
    before_dot = False
    nums_before_dot = 0
    nums_after_dot = 0
    dots = 0
    while len(string) > 0:
        valid = False
        for keyword in keywords[::-1]:
            if string.endswith(keyword):
                if keyword == "setin" and string.endswith("desetin"):
                    keyword = "desetin"
                if before_dot:
                    nums_before_dot += 1
                if not before_dot and keyword not in ["cela", "cele", "celych"]:
                    nums_after_dot += 1
                if keyword in ["cela", "cele", "celych"]:
                    before_dot = True
                    dots += 1
                    if dots > 1:
                        return False
                string = string[:len(string) - len(keyword)]
                valid = True
                break
        if not valid:
            return False
    if nums_before_dot > 0 and nums_after_dot > 0:
        return True
    return False


def validate_num(string: str):
    dots = 0
    nums = 0
    nums_after = 0
    for let in string:
        if let not in ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "."]:
            return False
        if dots == 0 and let != ".":
            nums += 1
        if dots == 1 and let != ".":
            nums_after += 1
        if let == ".":
            dots += 1
            if dots > 1:
                return False

    if nums > 0 and nums_after > 0:
        return True
    return False


if not input_string.isalpha():
    if validate_num(input_string):
        before, after = formatNums(input_string)
        numToStr(before, after)
    else:
        print("ERROR")
        quit()
elif input_string.isalpha():
    if validate_input(input_string):
        strToNum(input_string)
    else:
        print("ERROR")
        quit()

