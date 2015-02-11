loadI   1024    =>  r0
loadI   5       =>  r1
loadI   1       =>  r2
loadI   1       =>  r3
start:
mult    r1, r2  =>  r2
subI    r1, 1   =>  r1
cmp_GT  r1, r3  =>  r4
cbr     r4      =>  start, done
done:
storeAI r2      => r0, -4
output  1020
