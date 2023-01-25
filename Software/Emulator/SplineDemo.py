# https://catxmai.github.io/pdfs/Math212_ProjectReport.pdf

import numpy as np
import matplotlib.pyplot as plt


def interpolate(data, tension):
    pointsX = []
    pointsY = []
    for i in range(len(data)):
        if(data[i] >= 0.0):          # Get points that must be interpolated
            pointsX.append(i)
            pointsY.append(data[i])

    stretch = 4.5
    tension = np.clip((tension / stretch + (1 - (1 / stretch))), 0.0, 1.0)

    tau = 10**(tension * 7) / 10000000
    n = len(pointsX) - 1
    
    # corresponding to h, alpha , beta , gamma . Lists of n 0s
    h, a, b, g = [0] * n, [0] * n, [0] * n, [0] * n
    
    # calculating the variables
    for i in range(n):
        h[i] = pointsX[i + 1] - pointsX[i]
        g[i] = tau**2 * (pointsY[i + 1] - pointsY[i]) / h[i]
        a[i] = 1 / h[i] - tau / np.sinh(tau * h[i])
        b[i] = tau * np.cosh(tau * h[i]) / np.sinh(tau * h[i]) - 1 / h[i]
    
    # initializing A,Z,Y matrices
    A = np.zeros((n + 1, n + 1))
    for row in range(1, n):
        A[row][row - 1] = a[row - 1]
        A[row][row] = b[row - 1] + b[row]
        A[row][row + 1] = a[row]
        A[0][0] = 1
        A[n][n] = 1
    
    Y = np.zeros((n + 1, 1))
    for i in range(1, n):
        Y[i][0] = g[i] - g[i - 1]
    
    # convert python list to matrix
    Y = np.asmatrix(Y)
    A = np.asmatrix(A)
    
    # use inverse of matrix to solve for Z
    z = np.linalg.inv(A) * Y
    
    def getSection(x):
        i = 0
        while pointsX[i] <= x:
            i += 1
            if i == len(pointsX):
                break
        i -= 1
        return min(i, len(pointsX) - 2)
    
    # calculating the actual polynomial
    def f(x):
        i = getSection(x)       # Get current section offset
        
        t1 = (z[i] * np.sinh(tau * (pointsX[i + 1] - x))) + (z[i + 1] * np.sinh(tau * (x - pointsX[i])))
        t1 = t1 / (tau**2 * np.sinh(tau * h[i]))
        t2 = (pointsY[i] - z[i] / tau**2) * (pointsX[i + 1] - x) / h[i]
        t3 = (pointsY[i + 1] - z[i + 1] / tau**2) * (x - pointsX[i]) / h[i]
        return float(t1 + t2 + t3)
    
    
    out = np.copy(data) # Copy input data to output, negative values get replaced
    for i in range(len(out)):
        if(data[i] < 0.0):
            out[i] = f(i)
            
    return out
    


COUNT = 288

x = np.ones(COUNT) * (-1)
# x[:10] = 1.0
# x[200:] = 0.5
# x[50] = 0.3
# x[55] = 0.2

# x[99] = 0.0
# x[100] = 0.9
# x[101] = 1.0
# x[102] = 0.9
# x[150] = 0.0


x[:50] = 1.0
# x[100:150] = 0.8
x[100] = 0.3
x[150] = 0.3

x[180] = 0.8
x[-50:] = 0.0



original = np.clip(x, 0.0, 1.0)
plt.plot(original, ".")

for t in [0.0, 0.2, 0.4, 0.6, 0.8, 1.0]:
    plt.plot(np.clip(interpolate(x, t), 0.0, 1.0))#, ".-")
plt.show()
