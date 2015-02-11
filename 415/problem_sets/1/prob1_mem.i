loadI   1024    =>  r0
loadI   5       =>  r1
storeAI r1      =>  r0, 4
loadI   1       =>  r1
storeAI r1      =>  r0, 8

start:
loadAI  r0, 0   =>  r1
loadAI  r0, 4   =>  r2
mult    r1, r2  =>  r2
storeAI r2      =>  r0, 8

loadAI  r0, 0   =>  r1
subI    r1, 1   =>  r1
storeAI r1      =>  r0, 4

loadI   1       =>  r1
loadAI  r0, 0   =>  r2
cmp_GT  r2, r1  =>  r1
storeAI r1      =>  r0, 16

loadAI  r0, 8   =>  r1
cbr     r1      =>  start, done
done:
loadAI  r0, 4   =>  r1
storeAI r1      =>  r0, -4
output  1020
