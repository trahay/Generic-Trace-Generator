import sys
import os

try:
    from hashlib import md5
except:
    from md5 import new as md5

if len(sys.argv) == 1:
    print>>sys.stderr, 'Invalid number of arguments'
    sys.exit(3)

m = md5()
hexdigest_length = m.digest_size * 2

if sys.argv[1] in ('--check', '--check-output', '--check-input'):
    if len(sys.argv) != 3:
        print>>sys.stderr, 'Invalid number of arguments'
        sys.exit(3)

    # next arg is sig file
    signame = sys.argv[2]

    # read the sigfile and the two signatures for the output and input
    fp = open(signame)
    sig = fp.read()
    fp.close()
    outsig = sig[0:hexdigest_length]
    insig  = sig[hexdigest_length+1:2 * hexdigest_length+1]
    filenames = [x.decode('string_escape') for x in sig[2 * hexdigest_length + 2:].split('\n') if x]

    sigdir = os.path.dirname(signame)
    # than the output file
    outname = filenames[0]
    # remaings are the input files
    infiles = filenames[1:]

    if sys.argv[1] in ('--check', '--check-output'):
        filepath = os.path.normpath(os.path.join(sigdir, outname))
        fp = open(filepath)
        m.update(fp.read())
        fp.close()
        if m.hexdigest() != outsig:
            #print>>sys.stderr, 'Outout does not match signature: %s <> %s' % (m.hexdigest(), outsig)
            sys.exit(1)
        m = md5()

    if sys.argv[1] in ('--check', '--check-input'):
        for name in infiles:
            filepath = os.path.normpath(os.path.join(sigdir, name))
            fp = open(filepath)
            m.update(fp.read())
            fp.close()
        if m.hexdigest() != insig:
            #print>>sys.stderr, 'Inputs does not match signature: %s <> %s' % (m.hexdigest(), insig)
            sys.exit(2)
        m = md5()

    sys.exit(0)

# generate mode

if len(sys.argv) < 2:
    print>>sys.stderr, 'Invalid number of arguments'
    sys.exit(3)

# second arg is output name
#print>>sys.stderr, 'out: %s' % (sys.argv[2],)
fp = open(sys.argv[1])
m.update(fp.read())
fp.close()
outsig = m.hexdigest()
m = md5()

# remainging files are input
for name in sys.argv[2:]:
    #print>>sys.stderr, 'in:  %s' % (name,)
    fp = open(name)
    m.update(fp.read())
    fp.close()
insig = m.hexdigest()

# write sig file
sys.stdout.write('%s\n%s\n' % (outsig, insig))
for name in sys.argv[1:]:
    sys.stdout.write('%s\n' % (name.encode('string_escape'),))

sys.exit(0)
