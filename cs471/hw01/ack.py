"""
    ack( m,n ) =
        n + 1                   if m = 0
        ack(m - 1, 1)           if n = 0 and m > 0 
        ack(m-1, ack(m, n-1))   if n >0 and m > 0
"""

mem = {}
def ack(m, n):
    if (m, n) not in mem:
        mem[(m,n)] = n + 1 if m == 0 else (
            ack(m - 1, 1) if n == 0 else (
            ack(m - 1, ack(m, n - 1) )))
    return mem[(m,n)]

print('ack(1,0) =', ack(1,0))
print('ack(0,3) =', ack(0,3))

print('\nWould you like to attempt to calculate large Ackermann numbers?')
if input().lower()[0] == 'y':
    for i in range(500, 1000000, 500):
        print('ack(2,{}) = {}'.format(i, ack(2,i)))
