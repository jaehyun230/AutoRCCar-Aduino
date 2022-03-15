f = open('connect.inp','r')


F=f.readlines()

X = [ [int(y) for y in x.split()] for x in F]

#A=X[0],  B=X[1], C=X[2]

def distance(A,B):
    distance=((A[0]-B[0])*(A[0]-B[0])+(A[1]-B[1])*(A[1]-B[1])+(A[2]-B[2])*(A[2]-B[2]))**0.5
    return distance

# A=X[0], B=X[1]
def center(A,B):
    C=[(A[0]+B[0])/2,(A[0]+B[0])/2, (A[0]+B[0])/2]
    return C


while(True):
    A_distance=distance(X[0],X[2])
    B_distance=distance(X[1],X[2])
    if (-1< A_distance-B_distance <2):
        test= int(A_distance)+1

        break
    else:
        if A_distance > B_distance:
            X[0]=center(X[0],X[1])
        else:
            X[1]=center(X[0],X[1])


test=str(test)
myf = open('connect.out', 'w')
myf.write(test)
myf.close()



def myLoof(A,B,C):

    A_distance=distance(X[0],X[2])
    B_distance=distance(X[1],X[2])
    if (-1< A_distance-B_distance <1):
        print int(distance)
    else:
        if A_distance > B_distance:
            A=center(A,B)
        else:
            B=center(A,B)


f.close()