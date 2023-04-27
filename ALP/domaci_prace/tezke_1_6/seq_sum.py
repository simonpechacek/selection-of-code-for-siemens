# najde a spočítá součet nejdelší sekvence neklesajících prvočísel => nums - sekvence
nums = list(map(int, input().split()))

def is_prime(num):
    num = abs(num)
    if num in (0, 1):
        return False
    if num == 2:
        return True
    for i in range(num):
        if i in(0, 1):
            continue
        
        if num % i == 0:
            return False
    return True

def bigger_than_prev(list_nums, num):
    
    if len(list_nums) > 0:
        if num >= list_nums[-1]:
            return True
        return False

    return True

def get_longest_streaks():
    streak = 0
    max_streak = 0
    longest_lists = []
    temp_list = []
    for i in nums:
        if is_prime(i) and bigger_than_prev(temp_list, i):
            streak += 1
            temp_list.append(i)

        else:
            if streak > max_streak:
                max_streak = streak
                longest_lists = [temp_list]
            elif streak == max_streak and max_streak != 0:
                longest_lists.append(temp_list)
            streak = 0
            temp_list = []
            if is_prime(i):
                streak +=1
                temp_list.append(i)
                
    if streak > max_streak:
        max_streak = streak
        longest_lists = [temp_list]
    elif streak == max_streak and max_streak != 0:
        longest_lists.append(temp_list)
            
    return longest_lists

def get_biggest_sum(streaks):
    summ = 0
    max_summ = 0
    if len(streaks) == 0:
        return 0, 0
    lenght = len(streaks[0])
    for lst in streaks:
        for i in lst:
            summ += i
        if summ > max_summ:
            max_summ = summ
            summ = 0
        elif max_summ == 0:
            max_summ = summ
            summ = 0
    return max_summ, lenght

max_streaks = get_longest_streaks()
max_summ, lenght = get_biggest_sum(max_streaks)
print(lenght)
print(max_summ)




            