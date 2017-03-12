#!/usr/bin/env python
import sys
import re


 

D={}
L_coord=[]

def main():
    
    #street=raw_input()
    p0=re.compile(r'^\s*([acrg]).*$')
    p1=re.compile(r'^\s*([ac])\s*(["].+["])\s*(\s*[(]\s*[-+]?\d+\s*,\s*[-+]?\d+\s*[)]\s*)+\s*$')
    p2=re.compile(r'^\s*([r])\s*$')
    p3=re.compile(r'^\s*([g])\s*$')

    t=raw_input()
    
    m0=p0.match(t)
    m1=p1.match(t)   
    m2=p2.match(t)
    m3=p3.match(t)
    

    if m1:
        if m1.group(1)=='a':
            add_street(D,m1,t)
           # L_coord=D.values()
           # print "L_test:",L_coord
        elif m1.group(1)=='c':
            change_street(D,m1,t)
    elif m2:
        #remove_street(D,m2)

        D.clear()
        del L_coord[:]
        
    elif m3:
        gen_graph()
    else:
        errStr="Error: Illigal input.\n"
        sys.stderr.write(errStr)
        

            
def add_street(D,street,temp):
    global L_coord
         #to judge whether D is empty
    if D:
        #print 'D_test',D
        add=0
        for k,v in D.iteritems():
            #print "streetname:",k
            if k==street.group(2):
                #print 'street name:',street.group(2)
                add=1
                errSt="Error:'a'specified for a street %s that already exists \n"%(street.group(2))
                sys.stderr.write(errSt)
        if add==0:
            V=store_v(temp)                             
            if len(V)<2:
                errSt='Error:Incomplete coordinates in a %s, no end point. \n'%(street.group(2))
                sys.stderr.write(errSt)
            else:
                D.setdefault(street.group(2),V)
    else:
        V=store_v(temp)                             
        if len(V)<2:
            errSt='Error:Incomplete coordinates in a %s, no end point. \n'%(street.group(2))
            sys.stderr.write(errSt)
        else:
            D.setdefault(street.group(2),V)

    #print 'D:',D
    L_coord=D.values()
    #print 'L_coord:',L_coord
    '''else:
        errSt='Error: illegal input \n'
        sys.stderr.write(errSt)'''
    
def change_street(D,street,temp):
    global L_coord
    if D:
        add=0
        for k,v in D.iteritems():
            if k==street.group(2):
                add=1
        if add==1:            
            V=store_v(temp)                             
            if len(V)<2:
                errSt='Error:Incomplete coordinates in a %s, no end point. \n'%(street.group(2))
                sys.stderr.write(errSt)
            else:
                D[street.group(2)]=V
                #print 'D:',D
                L_coord=D.values()
                #print 'L_coord:',L_coord
        elif add==0:            
            errSt="Error:'c'specified for a street that does not exist. \n"
            sys.stderr.write(errSt)
    else:
        errSt="Error:'c'specified for a street that does not exist. \n"
        sys.stderr.write(errSt)
    
    
def remove_street(D,street):
    global L_coord
    if D:
        add=0
        for k,v in D.iteritems():
            if k==street.group(2):
                add=1
        if add==1:
            D.pop(street.group(2))
        elif add==0:
            errSt="Error:'r'specified for a street that does not exist. \n"
            sys.stderr.write(errSt)
            
    #print 'D:',D
    L_coord=D.values()
    #print 'L_coord',L_coord

            
            
        
def store_v(street):
    l_vetex=[]
    l_vetex1=[]
    p1=re.compile(r'^\s*([ac])\s*(["].+["])\s*(.*)$')
    a=p1.match(street)
    v=re.compile(r'\s*([+-]?\d+)s*')
    #print 'a.group:',a.group(3)


    for i in v.finditer(a.group(3)):
        #print i.group()
        l_vetex.append(int(i.group()))
             
    #print 'l_vetex:',l_vetex

    for j in range(len(l_vetex)):
        if j%2==0:
            l_vetex1.append((l_vetex[j],l_vetex[j+1]))                
    #print l_vetex1
    return l_vetex1

            
       
def gen_graph():
    global L_coord
    #print L_coord
    #a=[((2,-1),(2,2),(5,5),(5,6),(3,8)),((4,2),(4,8)),((1,4),(5,8))]
    lv=[]
    lv1=[]
    list2=[]
    list1=[]
    #dict2={}
    for i in range(0,len(L_coord)):
        for j in range(i+1,len(L_coord)):
            for m in range(0,len(L_coord[i])-1):
                for n in range(0,len(L_coord[j])-1):
                    R=lSegInt(L_coord[i][m],L_coord[i][m+1], L_coord[j][n],L_coord[j][n+1])
                    
                    if R:
                        #print "Intersection detected:", R
                        #store the intersection and segment end point in l
                        #print R
                        lv.append(R)
                        lv.append(L_coord[i][m])
                        lv.append(L_coord[i][m+1])
                        lv.append(L_coord[j][n])
                        lv.append(L_coord[j][n+1])
                        #to 
                        list1.append((L_coord[i][m],L_coord[i][m+1]))
                        list1.append(R)
                        list2.append(list1)
                        #list1.append(R)
                        list1=[]
                        list1.append((L_coord[j][n],L_coord[j][n+1]))
                        list1.append(R)
                        list2.append(list1)
                        
                        list1=[]
    
                            
    for i in lv:
        
        if i not in lv1:
            lv1.append(i)
    #print 'l_v:',lv1
            
            
    temp0=[]
    #remove duplicate element in list2
    for i in list2:
        if i not in temp0:
            temp0.append(i)
    list2=temp0
#to insert intersections into the same segment 
    
    list3=list2
    for i in range(len(list2)):
        for j in range(len(list3)):
            if list2[i][0]==list3[j][0] and list2[i][1]!=list3[j][1]:
                        list2[i].append(list3[j][1])
                        
    
    #print list2
#to sort the list 
    list3=[]
    list4=[]
    i=range(0,len(list2))

    for i in range(len(list2)):
    
        list2[i].append(list2[i][0][0])
        list2[i].append(list2[i][0][1])
        del list2[i][0]
    #print list2
    for i in list2:
        list3.append(sorted(i,key=getKey0))
    #print list3
    for i in list3:
        list4.append(sorted(i,key=getKey1))
    #print list4
#then remove duplicate element in list4
    l=[]
    for i in list4:
        if i not in l:
            l.append(i)
    #print 'street segment:',l  #l is street segment
    #remove duplicate elements in l, to consider origin
    le0=[]
    le1=[]
    for i in range(len(l)):
        for j in range(len(l[i])):
            if l[i][j] not in le0:
                le0.append(l[i][j])
        le1.append(le0)
    #print l1
        le0=[]
    #print le1
    l=le1
    #print 'non duplicate street segment:',l
    
#output Edges
    l1=[]
    for i in range(0,len(l)):
        for j in range(0,len(l[i])-1):
            l1.append([l[i][j],l[i][j+1]])
        
    #print 'pair of edges:',l1                   #l1 is pair of points, named edges

    '''#to make the points to dictionary
    d={}
    d1={}
    d2={}
    n=0
    for i in range(len(l)):
	for j in range(len(l[i])):
		n=n+1
		d1.setdefault(n,l[i][j])
		
        #print d1
	d.setdefault(i+1,d1)
	d1={}
    print 'dict:',d'''

    #print the fianl vertex output
    dv={}
    n=0
    #lv11=[]
    #convert point to float
    #for i in range(len(lv1)):
        #lv11.append((float(lv1[i][0],float(lv1[i][1]))
        
    for i in range(len(lv1)):
		n=n+1		
	        dv.setdefault(i+1,lv1[i])

    #lv1 is the right V list        
    #print 'lv1:',lv1
    '''print 'V={'
    for k,v in dv.iteritems():
        print k,':','(',round(v[0],2),',',round(v[1],2),')'
    print '}' '''

    #print the correct V format for assignment3
    sys.stdout.write('V '+str(len(lv1))+'\n')
    
    #print the fianl edge output
    de1={}
    de2={}

    for i in range(len(l1)):
        for j in range(len(l1[i])):
            for k in range(len(lv1)):
                if l1[i][j]==lv1[k]:
                    de1.setdefault(k,lv1[k])
        de2.setdefault(i+1,de1)
        de1={}
    #print 'E:',de2
    ll=de2.values()
    ll1=[]
    for i in range(len(ll)):
        ll1.append(ll[i].keys())
    '''print 'E={'
    #print '{'
    for i in range(len(ll1)):
        if i in range(len(ll1)-1):
            print '<',ll1[i][0],',',ll1[i][1],'>',','
        elif i==len(ll1)-1:
            print '<',ll1[i][0],',',ll1[i][1],'>'
    print '}' '''

    #print the correct E format for assignment3
    sys.stdout.write('E'+' '+'{')
    for i in range(len(ll1)):
        if i in range(len(ll1)-1):
            sys.stdout.write('<'+str(ll1[i][0])+','+str(ll1[i][1])+'>'+',')
        elif i==len(ll1)-1:
            sys.stdout.write('<'+str(ll1[i][0])+','+str(ll1[i][1])+'>')
    sys.stdout.write('}'+'\n')

    sys.stdout.flush()

    

def getKey0(item):
    return item[0]
def getKey1(item):
    return item[1]

def lSegInt(s1, s2, t1, t2):
    if s1[0] != s2[0]:                # if s is not vertical
        b1 = (s2[1] - s1[1]) / float(s2[0] - s1[0])
        if t1[0] != t2[0]:             # if t is not vertical
            b2 = (t2[1] - t1[1]) / float(t2[0] - t1[0])
            a1 = s1[1] - (b1 * s1[0])
            a2 = t1[1] - (b2 * t1[0])
            if b1 == b2:                # if lines are parallel (slopes match)
                for i in [s1,s2]: #if parallel, judge if there is a intersection
                    for j in [t1,t2]:
                        if i==j:
                            
                            return i
                    
                else:
                    return(None)
            xi = -(a1-a2)/float(b1-b2)  # solve for intersection point
            yi = a1 + (b1 * xi)
        else:
            xi = t1[0]
            a1 = s1[1] - (b1 * s1[0])
            yi = a1 + (b1 * xi)
    else:
        xi = s1[0]
        if t1[0] != t2[0]:            # if t is not vertical
            b2 = (t2[1] - t1[1]) / float(t2[0] - t1[0])
            a2 = t1[1] - (b2 * t1[0])
            yi = a2 + (b2 * xi)
        else:
            return(None)
    # Here is the actual intersection test!
    if (s1[0]-xi)*(xi-s2[0]) >= 0 and \
    (s1[1]-yi)*(yi-s2[1]) >= 0 and \
    (t1[0]-xi)*(xi-t2[0]) >= 0 and \
    (t1[1]-yi)*(yi-t2[1]) >= 0:
        return((float(xi), float(yi)))  # Return the intersection point.
    else:
        return False       
        

while True:
    #print 'D:',D
    #print 'l_test',L_coord
    main()

