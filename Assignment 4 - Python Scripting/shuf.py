import argparse
import sys
import random

def find_dupe(commands):
    for x in commands:
        if x=="-r":
            commands.remove('-r')    
    return commands

def int_picker(low,high,done,infinite):
    output=random.randint(low,high)
    while(output in done):
        output=random.randint(low,high)
    print(output)
    if not infinite:
        done.append(output)
    
def rand_int_print(low,high,hcount,infinite,repeat):
    
    num = high-low+1
    if (hcount != -1 and hcount<=num and repeat==False):
        
        done=[]
        while(hcount!=0):
            int_picker(low,high,done,infinite)
            hcount = hcount-1
    elif(hcount != -1 and hcount>num and repeat==True):
        done=[]
        while(hcount!=0):
            int_picker(low,high,done,True)
            hcount = hcount-1
    elif(hcount != -1 and hcount<num and repeat==True):
        done=[]
        while(hcount!=0):
            int_picker(low,high,done,False)
            hcount = hcount-1
    else:
        done=[]
        while(len(done)!=(high-low+1)):
            int_picker(low,high,done,infinite)
        
def shuffle(file,stdin,hcount,infinite,repeat):
    switch_hcount=False
    count=0
    lines = file.split('\n')
    lines.pop(len(lines)-1)
    numlines = len(lines)
    
    if(hcount!=-1):
       switch_hcount=True

    if switch_hcount:
        count=hcount
        if (hcount>numlines and repeat==False):
            count=numlines
        if (hcount>numlines and repeat==True):
            count=hcount
    else:
        count=numlines

    while(count!=0):
        printline=random.randint(0,numlines-1)
        print(lines[printline])
        if not infinite:
            if(repeat==False):
                lines.pop(printline)
                numlines=numlines-1
            count=count-1
    
def makeargs(parser):
    parser.add_argument("-n","--head-count",type=int,help="Output at most count lines. By default, all input lines are output.")
    parser.add_argument("-i","--input-range", help="Act as if input came from a file containing the range of unsigned decimal integers lo...hi, one per line.")
    parser.add_argument("file",default="-",nargs='?')
    parser.add_argument("-r","--repeat",action="store_true",help="Repeat output values, that is, select with replacement.")
    
def check(args):
    file=None
    valid=True
    stdin=False
    hcount=-1
    repeat = args.repeat
    infinite=False
    s1 = True
    
    find_dupe(sys.argv)
    
    if(len(sys.argv)==4 and args.input_range and repeat):
        s1=False

    if(s1 and (len(sys.argv)>3) and args.file and args.input_range and (not args.head_count and args.head_count!=0)): 
        sys.exit("Can't have --input-range and file")

    if(args.file!='-'):
        with open(args.file) as f:
            file = f.read()
    
    if ((len(sys.argv)==1 or (len(sys.argv)==2 or (len(sys.argv)==3 and args.head_count) or (len(sys.argv)==3 and sys.argv[1]=='-n') or (len(sys.argv)==4)and args.head_count and repeat) and args.file=='-')):
        file=sys.stdin.read()
        stdin=True

    if args.head_count or args.head_count==0:
        if (args.head_count < 0):
           sys.exit("invalid line count") 
        else:
            hcount=args.head_count

    if((not hcount or hcount ==-1) and repeat and not (hcount==0)):
        infinite=True
    
    if args.input_range:
        valid=False
        try:
            string =args.input_range
            mid=-1
            mid=string.find('-')
            low=int(string[0:mid])
            high=int(string[mid+1:len(args.input_range)])
            
            if(mid==-1 or low<0 or low > high):
                raise Exception("")

            rand_int_print(low,high,hcount,infinite,repeat)
        except Exception:
            valid = False
            sys.exit("Invalid bounds")
    
    if valid:
        shuffle(file,stdin,hcount,infinite,repeat)

if __name__ == '__main__':
 parser = argparse.ArgumentParser()
 makeargs(parser)
 
 args = parser.parse_args()
 check(args)