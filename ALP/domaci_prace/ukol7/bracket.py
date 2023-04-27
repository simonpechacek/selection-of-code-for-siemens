"""
This program finds all possible combinations of brackets given numbers of each bracket type used
N -> ()
M -> []
O -> {}
"""
import sys
N = int(sys.argv[1])#int(input())
M = int(sys.argv[2])#int(input())
O = int(sys.argv[3])#int(input())

stack = []

opening_brackets = "(" * N + "[" * M + "{" * O
closing_brackets = ")" * N + "]" * M + "}" * O
closing_dict = {
    "(": ")",
    "[": "]",
    "{": "}"
}


def new_possibility(legal_brackets):
    if not legal_brackets:
        for br in stack:
            print(br, end="")
        print()
    prev = ""
    for brack in legal_brackets:
        if brack != prev:
            stack.append(brack)
            new_possibility(legal())
            stack.pop()
            prev = brack


def legal():
    n = m = o = 0
    for br in stack:
        if br == "(":
            n += 1
        elif br == "[":
            m += 1
        elif br == "{":
            o += 1
    legal_brackets = ""
    for _ in range(n, N):
        legal_brackets += "("
    for _ in range(m, M):
        legal_brackets += "["
    for _ in range(o, O):
        legal_brackets += "{"
    closed = ["", 0]
    for brack in range(len(stack) - 1, -1, -1):
        if stack[brack] in closing_dict:
            if not closed[0]:
                legal_brackets += closing_dict[stack[brack]]
                break
            else:
                if closing_dict[stack[brack]] == closed[0]:
                    closed[1] -= 1
                    if closed[1] == 0:
                        closed = ["", 0]
                    continue
        else:
            if not closed[0]:
                closed = [stack[brack], 1]
            elif closed[0] == stack[brack]:
                closed[1] += 1
    return legal_brackets

new_possibility(opening_brackets)

"""
for bracket in opening_brackets:
    if bracket != prev_bracket:
        stack.append(bracket)
        new_possibility(legal())
        stack.pop()
        prev_bracket = bracket
"""
