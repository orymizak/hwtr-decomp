#!/usr/bin/env python3
"""Cross-reference N64 functions with the PS1 build (same Stormfront source).

The PS1 CCCPSX.EXE shares much of the game code. This finds N64<->PS1 function
pairs through rare strings both functions use (anchors), then can print both
function sequences side by side, since functions keep source-file order.

Needs ghidra_work/decomp.jsonl and a folder holding CCCPSX.EXE plus Ghidra's
CCCPSX.EXE.c export (not distributable; not in the repo).

Usage:
  tools/ps1_xref.py <ps1_gamedata_dir>                         list anchors (* = still INCLUDE_ASM)
  tools/ps1_xref.py <dir> <n64_lo>,<n64_hi>,<ps1_lo>,<ps1_hi>  side-by-side window (hex)
"""
import re, json, os, collections, glob, sys
HW=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PSD=os.path.join(sys.argv[1],'')
rom=open(HW+'/baserom.z64','rb').read()
exe=open(PSD+'CCCPSX.EXE','rb').read()
def cstr(buf,off):
    if not 0<=off<len(buf): return None
    e=buf.find(b'\0',off); s=buf[off:e]
    if len(s)<3 or any(c<9 or c>126 for c in s): return None
    return s.decode()
# N64 vram->rom: main segment 0x80060400 at rom 0x1000
n64s=lambda a: cstr(rom,a-0x80060400+0x1000)
pss=lambda a: cstr(exe,a-0x80010000+0x800)
# N64 names
names={}
for l in open(HW+'/symbol_addrs.txt'):
    m=re.match(r'\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+);.*type:func',l)
    if m: names[int(m.group(2),16)]=m.group(1)
rem=set()
for f in glob.glob(HW+'/src/*.c'):
    rem|=set(re.findall(r'INCLUDE_ASM\("[^"]+",\s*(\w+)\)',open(f).read()))
N={}
for l in open(HW+'/ghidra_work/decomp.jsonl'):
    j=json.loads(l); a=int(j['addr'],16)
    refs=set(int(x,16) for x in re.findall(r'\b(?:s|PTR_s|DAT|PTR_DAT)_\w*?_?(80[0-9a-fA-F]{6})\b',j['c']))
    refs|=set(int(x,16) for x in re.findall(r'\bD_(80[0-9A-F]{6})',j['c']))
    N[a]=dict(name=names.get(a,j['name']),strs={s for r in refs if (s:=n64s(r))},calls=re.findall(r'\b(\w+)\(',j['c']))
ps=open(PSD+'CCCPSX.EXE.c',errors='replace').read()
P={}
for m in re.finditer(r'\n[\w\* ]+?\bFUN_([0-9a-f]{8})\([^)\n]*\)\n\n?\{\n(.*?)\n\}\n',ps,re.S):
    refs=set(int(x,16) for x in re.findall(r'\b(?:s|PTR_s|DAT|PTR_DAT|PTR_s)_\w*?_?(80[0-9a-f]{6})\b',m.group(2)))
    P[int(m.group(1),16)]=dict(strs={s for r in refs if (s:=pss(r))},body=m.group(2))
# string rarity
cntN=collections.Counter(s for v in N.values() for s in v['strs'])
cntP=collections.Counter(s for v in P.values() for s in v['strs'])
pairs=[]
for a,v in N.items():
    for b,w in P.items():
        k=v['strs']&w['strs']
        if not k: continue
        score=sum(1/(cntN[s]*cntP[s]) for s in k if len(s)>=3)
        pairs.append((score,a,b,sorted(k)))
# best per N64 func
best={}
for sc,a,b,k in sorted(pairs,reverse=True):
    if a not in best: best[a]=(sc,b,k)
anch=sorted((a,b,sc) for a,(sc,b,k) in best.items() if sc>=0.5)
print('N64 funcs',len(N),'PS funcs',len(P),'anchors',len(anch))
for a,b,sc in anch: print(hex(a),N[a]['name'][:28].ljust(28),hex(b),round(sc,2),'*' if N[a]['name'] in rem else '',best[a][2][:2])

def seq(D,lo,hi): return [a for a in sorted(D) if lo<=a<=hi]
def show(nlo,nhi,plo,phi):
    print('\n=== N64 %x-%x  |  PS1 %x-%x'%(nlo,nhi,plo,phi))
    ns=seq(N,nlo,nhi); pss_=seq(P,plo,phi)
    for i in range(max(len(ns),len(pss_))):
        l=('%x %s%s'%(ns[i],N[ns[i]]['name'][:30],' *' if N[ns[i]]['name'] in rem else '')) if i<len(ns) else ''
        r=('%x lines=%d'%(pss_[i],P[pss_[i]]['body'].count('\n'))) if i<len(pss_) else ''
        print(l.ljust(50),r)
for spec in sys.argv[2:]:
    show(*[int(x,16) for x in spec.split(',')])
