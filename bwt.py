import sys
ref = ""
reads = []
with open(sys.argv[1]) as fp:
    line = fp.readline().rstrip()
    ref = line[::-1]
    line = fp.readline().rstrip()
    while line:
        pos,seq = line.split()
        reads.append({"pos":len(ref)-int(pos),"seq":seq[::-1]})
        line = fp.readline().rstrip()

#Build SA
sa = [i for i in range(len(ref))]
sa = sorted(sa, key=lambda i: ref[i:])

def insert_reads(read, read_number, pos_in_read):
    a = 0
    b = len(sa)-1
    while a != b:
        m = (a+b)//2+1
        is_smaller = True
        if isinstance(sa[m], int):
            #we are comparing with a suffix of ref
            is_smaller = read["seq"][pos_in_read:] + ref[read["pos"]:] < ref[sa[m]:]
        else:
            #we are comparing with a suffix of a read
            other_read = reads[sa[m]["read_number"]]
            other_pos_in_read = sa[m]["pos_in_read"]
            is_smaller = read["seq"][pos_in_read:] + ref[read["pos"]:] < \
            other_read["seq"][other_pos_in_read:] + ref[other_read["pos"]:]
        if is_smaller:
            b = m - 1
        else:
            a = m
    sa.insert(a+1,{"read_number":read_number,"pos_in_read":pos_in_read})

start_of_read = [[] for _ in range(len(ref)+1)]
for i in range(len(reads)):
    start_of_read[reads[i]["pos"]].append(i)
    for j in range(len(reads[i]["seq"])):
        pos = insert_reads(reads[i],i,j)


#Build BWT
bwt = [ref[-1]]
for i_read in start_of_read[len(ref)]:
    bwt.append(reads[i_read]["seq"][-1])
for i in range(len(sa)):
    if isinstance(sa[i],int):
        if sa[i] > 0:
            bwt.append(ref[sa[i]-1])
            for i_read in start_of_read[sa[i]]:
                bwt.append(reads[i_read]["seq"][-1])
    else:
        if sa[i]["pos_in_read"] > 0:
            i_read = sa[i]["read_number"]
            bwt.append(reads[i_read]["seq"][sa[i]["pos_in_read"]-1])
print(bwt)
