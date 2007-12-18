import os
import re

ENDIAN, BITS, RWINDOW = 0, 1, 2

cpus = {"ia64":  ("little", 64, True),
        "ppc":   ("big",    32, False),
        "ppc64": ("big",    64, False),
        "s390":  ("little", 32, False),
        "s390x": ("little", 64, False)}

modes = {"ia64":  ("ia64",),
         "ppc":   ("ppc", "ppc64"),
         "s390":  ("s390", "s390x")}
                  
def isLittleEndian(cpu):
    if cpus[cpu][ENDIAN] == "little":
        return True
    return ""

def is64bit(cpu):
    # Only use this one for files with CPUS in their paths
    if cpus[cpu][BITS] == 64:
        return True
    return ""

def isRegisterWindow(cpu):
    if cpus[cpu][RWINDOW]:
        return True
    return ""

def preprocess(src, cpu):
    other_cpus = [key.upper() for key in modes.keys() if key != cpu]
    cpu = cpu.upper()
    things = re.compile(r"(.*?)(/\*|//|^[ \t]*#)(.*)", re.S | re.M)
    ends = {"/*": "*/"}
    COPY, COND_TRUE, COND_FALSE = 1, 2, 3
    mode = [COPY]
    dst = ""
    while src:
        thing = things.match(src)
        if not thing:
            if COND_FALSE not in mode:
                dst += src
            break
        before, thing, src = thing.groups()
        src = thing + src
        if COND_FALSE not in mode:
            dst += before
        end = ends.get(thing, "\n")
        index = src.find(end)
        assert index >= 0
        index += len(end)
        thing = src[:index]
        src = src[index:]
        if not thing.startswith("#"):
            if COND_FALSE not in mode:
                dst += thing
            continue
        args = thing.strip().split()
        cmd = args.pop(0)
        if cmd == "#":
            cmd += args.pop(0)

        if cmd in ("#include", "#define", "#undef"):
            if COND_FALSE not in mode:
                dst += thing
        elif cmd in ("#ifdef", "#ifndef"):
            us, them = {
                "#ifdef": (COND_TRUE, COND_FALSE),
                "#ifndef": (COND_FALSE, COND_TRUE)}[cmd]
            [what] = args
            if what == cpu:
                mode.append(us)
            elif what in other_cpus:
                mode.append(them)
            else:
                mode.append(COPY)
                if COND_FALSE not in mode:
                    dst += thing
        elif cmd == "#if":
            for check in [cpu] + other_cpus:
                assert "defined(%s)" % check not in args
            mode.append(COPY)
            dst += thing
        elif cmd == "#else":
            if mode[-1] == COND_TRUE:
                mode[-1] = COND_FALSE
            elif mode[-1] == COND_FALSE:
                mode[-1] = COND_TRUE
            else:
                if COND_FALSE not in mode:
                    dst += thing
        elif cmd == "#endif":
            if mode[-1] == COPY and COND_FALSE not in mode:
                dst += thing
            mode.pop()
        else:
            assert False

    assert mode == [COPY]
    if cpu == "PPC":
        dst = dst.replace("_LP64", "PPC64")
    return dst

def untabify(line):
    bits = line.split("\t")
    line = bits.pop(0)
    for bit in bits:
        line += " " * (8 - len(line) % 8)
        line += bit
    return line

def template(src, dst, basecpu, cpu):
    bits = open(src, "r").read().split("@@")
    assert len(bits) & 1
    for i in xrange(1, len(bits), 2):
        result = eval(bits[i].strip())
        if not result:
            result = ""
        bits[i] = result
    result = "".join(bits)
    if src.split(".")[-1] in ("c", "h", "cpp", "hpp"):
        result = preprocess(result, cpu)
    if src.split(".")[-1] != "c":
        result = "\n".join(
            [untabify(line.rstrip()) for line in result.split("\n")])
    if os.path.exists(dst):
        existing = open(dst, "r").read()
        if result == existing:
            return
    trim = os.getcwd() + os.sep
    assert dst.startswith(trim)
    print "creating", dst[len(trim):]
    dir = os.path.dirname(dst)
    if not os.path.isdir(dir):
        os.makedirs(dir)
    open(dst, "w").write(result)

def skip(item):
    prefixes, suffixes = ["."], ["~", ".orig", ".rej"]
    for prefix in prefixes:
        if item.startswith(prefix):
            return True
    for suffix in suffixes:
        if item.endswith(suffix):
            return True
    return False

def visit((srcdir, dstdir, cpus), dir, items):
    if ".hg" in items:
        items.remove(".hg")
    for item in items:
        if skip(item):
            continue
        path = os.path.join(dir, item)
        if not os.path.isfile(path):
            continue
        if path.find("CPU") == -1:
            continue
        multi = path.find("CPUS") >= 0
        trim = srcdir + os.sep
        assert path.startswith(trim)
        trim = len(trim)
        for basecpu in cpus:
            for cpu in modes[basecpu]:
                template(
                    path, os.path.join(dstdir,path[trim:] \
                                           .replace("CPUS", cpu) \
                                           .replace("CPU", basecpu)),
                    basecpu, cpu)
                if not multi:
                    break

if __name__ == "__main__":
    import sys
    srcdir = os.path.dirname(os.path.abspath(sys.argv[0]))
    dstdir = os.path.join(os.getcwd(), "ports")
    if not os.path.isdir(dstdir):
        print >>sys.stderr, "run me within the IcedTea toplevel directory"
        sys.exit(1)
    if len(sys.argv) < 2:
        cpus = modes.keys()
        cpus.sort()
        cpus = "|".join(cpus)
        print >>sys.stderr, "usage: %s %s..." % (sys.argv[0], cpus)
        sys.exit(1)
    os.path.walk(srcdir, visit, (srcdir, dstdir, sys.argv[1:]))
