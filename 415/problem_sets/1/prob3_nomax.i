loadI   1024    =>  r0
loadI   1       =>  r1
loadI   2       =>  r3
storeAI r3      =>  r0, 20
loadAI  r0, 20  =>  r3
subI    r3, 4   =>  r3
store   r3      =>  r0
loadAI  r0, 20  =>  r3
add     r1, r3  =>  r3
storeAI r3      =>  r0, 24
loadAI  r0, 24  =>  r3
addI    r3, 1   =>  r2
storeAI r2      =>  r0, 28
load    r0      =>  r3
loadAI  r0, 28  =>  r2
mult    r3, r2  =>  r3
storeAI r3      =>  r0, 4
loadAI  r0, 4   =>  r3
loadAI  r0, 28  =>  r2
sub     r2, r3  =>  r3
storeAI r3      =>  r0, 8
loadAI  r0, 24  =>  r3
loadAI  r0, 28  =>  r2
add     r3, r2  =>  r3
storeAI r3      =>  r0, 12
loadAI  r0, 8   =>  r3
loadAI  r0, 12  =>  r2
add     r2, r3  =>  r3
storeAI r3      =>  r0, 16
loadAI  r0, 16  =>  r3
add     r3, r1  =>  r3
storeAI r3    =>  r0, -4
output  1020
