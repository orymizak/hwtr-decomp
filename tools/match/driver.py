#!/usr/bin/env python3
import os,re,subprocess,sys,struct
S=os.path.dirname(os.path.abspath(__file__))
rom=open('baserom.z64','rb').read()
R=lambda v:v-0x80060400+0x1000
def files():
    out=[]
    y=open('hotwheelsturbo.yaml').read()
    for f in os.listdir('asm'):
        if not f.endswith('.s') or f in('1000.s','header.s'): continue
        t=open('asm/'+f).read()
        F=[]
        for m in re.finditer(r"nonmatching (\S+?), (0x[0-9A-F]+)\n\nglabel \S+\n(?:\s*\.L\S+:\n)*\s+/\* ([0-9A-F]+) ([0-9A-F]{8})",t):
            F.append((m.group(1),int(m.group(2),16),int(m.group(4),16)))
        F.sort(key=lambda x:x[2])
        cur=[]
        for i,x in enumerate(F):
            if i and x[2]%16==0 and F[i-1][2]+F[i-1][1]<x[2]:
                out.append(cur); cur=[]
            cur.append(x)
        if cur: out.append(cur)
    return out
limit=int(sys.argv[1]) if len(sys.argv)>1 else 999
done=set()
for it in range(limit):
    fl=[c for c in files() if R(c[0][2]) not in done]
    if not fl: break
    fl.sort(key=lambda c:c[-1][2]+c[-1][1]-c[0][2])
    c=fl[0]
    a=R(c[0][2]); e=c[-1][2]+c[-1][1]
    e=(e+15)&~15
    b=R(e)
    done.add(a)
    w=rom[a:b]; ws=struct.unpack(">%dI"%(len(w)//4),w)
    fp=sum(x==0x03A0F021 for x in ws)
    opt='-O0' if fp*2>=len(c) else '-O2'
    r=subprocess.run([sys.executable,S+'/apply.py',f'unit_{a:05X}',hex(a),hex(b),opt],capture_output=True,text=True)
    print(f'{a:05X} {b:05X} {len(c)} func {opt}:',r.stdout.strip().replace('\n',' | ')[-160:],r.stderr[-200:],flush=True)
