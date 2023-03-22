import numpy as np


class Utility():
    def hsvToRgb(h, s, v):
        h = np.mod(np.array([h]) if isinstance(h, float) else h, 1.0)
        s = np.clip(np.array([s]) if isinstance(s, float) else s, 0.0, 1.0)
        v = np.clip(np.array([v]) if isinstance(v, float) else v, 0.0, 1.0)
        n = np.size(h)
        r = np.zeros(n)
        g = np.zeros(n)
        b = np.zeros(n)
        
        for i in range(n):
            if s[i] == 0.0:
                r[i] = v[i]; g[i] = v[i]; b[i] = v[i]
            h[i] = (h[i] % 1.0) * 6.0
            j = int(h[i])
            f = (h[i]) - j
            p, q, t = v[i] * (1.0 - s[i]), v[i] * (1.0 - s[i] * f), v[i] * (1.0 - s[i] * (1.0 - f))
            j %= 6
            if j == 0:
                r[i] = v[i]; g[i] = t; b[i] = p
            elif j == 1:
                r[i] = q; g[i] = v[i]; b[i] = p
            elif j == 2:
                r[i] = p; g[i] = v[i]; b[i] = t
            elif j == 3:
                r[i] = p; g[i] = q; b[i] = v[i]
            elif j == 4:
                r[i] = t; g[i] = p; b[i] = v[i]
            elif j == 5:
                r[i] = v[i]; g[i] = p; b[i] = q
            
        r = r[0] if n == 1 else r
        g = g[0] if n == 1 else g
        b = b[0] if n == 1 else b
        return r, g, b

    
    def rgbToHsv(r, g, b):
        r = np.clip(np.array([r]) if isinstance(r, float) else r, 0.0, 1.0)
        g = np.clip(np.array([g]) if isinstance(g, float) else g, 0.0, 1.0)
        b = np.clip(np.array([b]) if isinstance(b, float) else b, 0.0, 1.0)
        n = np.size(r)
        h = np.zeros(n)
        s = np.zeros(n)
        v = np.zeros(n)
        
        for i in range(n):
            mx = max(r[i], g[i], b[i])
            mn = min(r[i], g[i], b[i])
            df = mx-mn
            if mx == mn:
                h[i] = 0
            elif mx == r[i]:
                h[i] = (60 * ((g[i]-b[i])/df) + 360) % 360
            elif mx == g[i]:
                h[i] = (60 * ((b[i]-r[i])/df) + 120) % 360
            elif mx == b[i]:
                h[i] = (60 * ((r[i]-g[i])/df) + 240) % 360
            if mx == 0:
                s[i] = 0
            else:
                s[i] = df/mx
            v[i] = mx
            h[i] /= 360
            
        h = h[0] if n == 1 else h
        s = s[0] if n == 1 else s
        v = v[0] if n == 1 else v
        return h, s, v


    def interpolateSpline(data, tension):   # TODO: Check performance -> Currently very slow :(
        firstX = lastX = -1.0       # Set start end end points to corner values (avoid empty borders)
        for i in range(len(data)):
            if(data[i] >= 0) and (firstX < 0):
                firstX = i
            if(data[len(data) - 1 - i] >= 0) and (lastX < 0):
                lastX = len(data) - 1 - i
        if firstX < 0.0 or lastX < 0.0:
            return np.clip(data, 0.0, 1.0)
        data[0:firstX] = data[firstX]
        data[lastX:] = data[lastX]
        
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
                
        return np.clip(out, 0.0, 1.0)
    
    
    def interpolate(data):
        firstX = lastX = -1.0       # Set start end end points to corner values (avoid empty borders)
        for i in range(len(data)):
            if(data[i] >= 0) and (firstX < 0):
                firstX = i
            if(data[len(data) - 1 - i] >= 0) and (lastX < 0):
                lastX = len(data) - 1 - i
        if firstX < 0.0 or lastX < 0.0:
            return np.clip(data, 0.0, 1.0)
        data[0:firstX] = data[firstX]
        data[lastX:] = data[lastX]
        
        i = 0
        while True:
            if(data[i] >= 0):
                x0 = i
                y0 = data[x0]
                
            i += 1
            if(i == len(data)):
                break
                
            if(data[i] >= 0) and x0 < i - 1:
                x1 = i
                y1 = data[x1]
                dx = x1 - x0
                dy = y1 - y0
                for xn in range(dx):
                    data[x0 + xn] = y0 + (dy / dx) * xn

        return np.clip(data, 0.0, 1.0)
    


if __name__ == '__main__':
    import matplotlib.pyplot as plt
    
    Utility.rgbToHsv(0.3, 0.276, 0.24)
    Utility.rgbToHsv(np.array([0.1, 0.2]), np.array([0.3, 0.4]), np.array([0.5, 0.6]))
    
    Utility.hsvToRgb(0.1, 0.2, 0.3)
    Utility.hsvToRgb(np.array([0.1, 0.2]), np.array([0.3, 0.4]), np.array([0.5, 0.6]))
    
    
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
    
    # x[49] = 1.0
    x[50] = 1.0
    # x[100:150] = 0.8
    x[100] = 0.3
    x[150] = 0.3
    
    x[180] = 0.8
    x[-50] = 0.0
    
    
    
    original = np.clip(x, 0.0, 1.0)
    plt.plot(original, ".")
    
    plt.plot(Utility.interpolate(x))
    # for t in [0.0, 0.2, 0.4, 0.6, 0.8, 1.0]:
    #     plt.plot(np.clip(Utility.interpolateSpline(x, t), 0.0, 1.0))
    plt.show()
