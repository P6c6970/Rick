import random

def zeml():
    return random.choice(['0','1'])
    #return "0"

x=27
y=26

kvadrat=[0,0,11,7]

for iter in range(y):
    if iter==0 or iter+1==y:
        print('-'*x)
    else:
        str=''
        for itertemp in range(x):
            if itertemp==0 or itertemp+1==x:
                str+='-'
            else:
                #if iter==kvadrat[1] or iter==kvadrat[1]+kvadrat[3]:
                #    if itertemp>=kvadrat[0] and itertemp<=kvadrat[0]+kvadrat[2]:
                #        str+='-'
                #    else:
                #        str+=zeml()
                #elif iter>=kvadrat[1] and iter<=kvadrat[1]+kvadrat[3]:
                #    if itertemp==kvadrat[0] or itertemp==kvadrat[0]+kvadrat[2]:
                #        str+='-'
                #    else:
                #        str+=zeml()
                #else:
                #    str+=zeml()
                str+=zeml()
        print(str)
