def pm(a):
    for r in range(len(a)):
        for s in range(len(a[0])):
            print(a[r][s], end = " ")
        print()

def gauss(m):
    R = len(m)
    S = len(m[0])
    for k in range(R):

        maxVal = m[k][k]
        maxIdx = k
        for r in range(k, R):
            if m[r][k] > maxVal:
                maxVal = m[r][k]
                maxIdx = r

        m[k], m[maxIdx] = m[maxIdx], m[k]

        for r in range(k+1, R):
            beta = -m[k][k] / m[r][k]
            for s in range(k, S):
                m[r][s] = beta*m[r][s] + m[k][s]
        print("k=",k)
        pm(a)
a = [[1,1,1,3],[2,0,8,10],[-1,0,0,-1]]
gauss(a)
