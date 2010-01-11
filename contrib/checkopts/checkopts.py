#!/usr/bin/env python

import os
import subprocess
import sys

class Flag:
    def __init__(self, line):
        line = line.split()
        self.type = line.pop(0)
        self.name = line.pop(0)
        assert line[0] in ("=", ":=")
        line = " ".join(line[1:])
        self.is_diagnostic = False
        if line.endswith("}"):
            index = line.rfind("{")
            if index != -1:
                self.is_diagnostic = "diagnostic" in line[index + 1:-1].split()
                line = line[:index]
        self.value = line.strip()

    @property
    def test_values(self):
        if self.type == "bool":
            if self.name in (
                "PrintFlagsInitial",    # already done :)
                "ExtendedDTraceProbes", # Solaris only
                "RequireSharedSpaces",  # Not set up
                "PauseAtStartup"):      # Just don't...
                return (False,)
            return (False, True)
        return ()
        
    def option(self, value):
        if self.type == "bool":
            return "-XX:%s%s" % (value and "+" or "-", self.name)
        elif self.type.endswith("intx"):
            return "-XX:%s=%d" % (self.name, value)
        raise ValueError, self.type

class TestFailure(Exception):
    pass

class Main:
    def __init__(self, java):
        self.java = os.path.realpath(java)
        self.base = os.path.dirname(os.path.realpath(sys.argv[0]))
        if not os.path.exists(os.path.join(self.base, "Test.class")):
            out, err = subprocess.Popen(
                [self.java + "c", "Test.java"],
                stdout = subprocess.PIPE,
                stderr = subprocess.PIPE,
                cwd    = self.base).communicate()
            if out or err:
                sys.stdout.write(out)
                sys.stderr.write(err)
                sys.exit(1)
        self.passes = self.fails = 0
        for flag in self.read_flags():
            for value in flag.test_values:
                self.test(flag, value)
        print "%d passes, %d fails" % (self.passes, self.fails)

    def read_flags(self):
        out, err = subprocess.Popen(
            [self.java,
             "-XX:+UnlockDiagnosticVMOptions",
             "-XX:+PrintFlagsInitial"],
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE).communicate()
        if err:
            sys.stderr.write(err)
            sys.exit(1)
        lines = out.rstrip().split("\n")
        line = lines.pop(0)
        if line != "[Global flags]":
            print >>sys.stderr, "error: unexpected output %s" % repr(line)
            sys.exit(1)
        flags = {}
        for line in lines:
            flag = Flag(line)
            assert not flags.has_key(flag.name)
            flags[flag.name] = flag
        flags = flags.items()
        flags.sort()
        return [flag for name, flag in flags]

    def test(self, flag, value):
        for dir in xrange(1000):
            dir = os.path.join(self.base, "results", flag.name, "%03d" % dir)
            if not os.path.isdir(dir):
                break
        os.makedirs(dir)
        cmd = [self.java]
        if flag.is_diagnostic:
            cmd.append("-XX:+UnlockDiagnosticVMOptions")
        if flag.name.startswith("CMS"):
            cmd.append("-XX:+UseConcMarkSweepGC")
        flag = flag.option(value)
        cmd.append(flag)
        cmd.extend(("-cp", self.base, "Test"))
        print "%-56s %s" % (flag, dir[len(self.base + os.sep):])
        print >>open(os.path.join(dir, "cmd"), "w"), " ".join(cmd)
        out, err = subprocess.Popen(
            cmd,
            stdout = subprocess.PIPE,
            stderr = subprocess.PIPE,
            cwd    = dir).communicate()
        open(os.path.join(dir, "out"), "w").write(out)
        open(os.path.join(dir, "err"), "w").write(err)
        try:
            self.check(dir, out, err)
            self.passes += 1
        except TestFailure, failure:
            print "\x1B[1;31m  FAIL: %s\x1B[0m" % failure
            self.fails += 1

    def fail(self, msg):
        raise TestFailure(msg)

    def check(self, dir, out, err):
        name = os.path.basename(os.path.dirname(dir))
        expect = ["cmd", "err", "out"]
        actual = os.listdir(dir)
        if name in ("LogCompilation", "LogVMOutput"):
            if "hotspot.log" in actual:
                expect.append("hotspot.log")
        if name == "PerfDataSaveToFile":
            for item in actual:
                if item.startswith("hsperfdata_"):
                    expect.append(item)
                    break
        expect.sort()
        actual.sort()
        while expect != actual:
            for item in actual:
                if item not in expect:
                    self.fail("found %s" % item)
            self.fail("directory contents not as expected")
        if err:
            self.fail("err not as expected")
        if not "Hello world" in out.split("\n"):
            self.fail("out not as expected")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print >>sys.stderr, "usage: %s /path/to/jdk/bin/java" % sys.argv[0]
        sys.exit(1)
    Main(sys.argv[1])
